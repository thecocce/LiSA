/*
 * ************** irred.cpp *******************************
 * 
 * test schedules for irreducibility
 *
 * Owner: Marc Moerig
 *
 * 21.02.01
 * last changes 27.03.01
 */

#include<stdlib.h>
#include<iostream.h>
#include<fstream.h>
#include"../../main/global.hpp"
#include"../../lisa/ctrlpara.hpp"
#include"../../scheduling/schedule.hpp"
#include"../../lisa/ptype.hpp"
#include"../../scheduling/mo_jo.hpp"
#include"../../lisa/lvalues.hpp"
#include"../../misc/except.hpp"
#include"graphalgo.hpp"
#include"convertgraph.hpp"
#include"irrednode.hpp"
#include"irredtest.hpp"

/** @name Irreducibility Test
   
This algorithm checks if a given schedule for a problem is irreducible. A schedule S* reduces another schedule 
S if its objective is better than that for S for any given set of processing types. S is therefore irreducible 
if no such schedule S* exists.\\

It works for open, job and flow shop problems, in combination with 
release dates and for the Cmax, Lmax, SumCi, SumWiCi, SumUi, SumWiUi, SumTi and SumWiTi objectives.

It may be called:\\

#irred [input file] [output file]#\\

Controlparameters are:

#string TYPE [SIMILAR ALL_REDUCING ITERATIVE_REDUCING]#\\
#string RETURN [ALL ONLY_IRREDUCIBLE]#\\
#string RNDM [YES NO]#\\

An example for a correct input file is:\\

#<PROBLEMTYPE>#\\
#Lisa_ProblemType= { O / r_i / Cmax }#\\
#</PROBLEMTYPE>#\\

#<CONTROLPARAMETERS>
#string TYPE ITERATIVE_REDUCING#\\
#string RETURN ALL#\\
#string RNDM YES#\\
#</CONTROLPARAMETERS>#\\
 
#<VALUES>#\\
#m= 2#\\
#n= 3#\\
#PT= {#\\
# { 25 82 }#\\
# { 88 59 }#\\
# { 57 10 }#\\
#}#\\
 
#SIJ= {#\\
# { 1 1 }#\\
# { 1 1 }#\\
# { 1 1 }#\\
#}#\\
 
#RD= { 48 5 9 }#\\
 
#</VALUES>#\\
 
#<SCHEDULE>#\\
#m= 2#\\
#n= 3#\\
#semiactive= 1#\\
#LR= {#\\
# { 3 2 }#\\
# { 2 1 }#\\
# { 4 3 }#\\
#}#\\
 
#CIJ= {#\\
# { 177 146 }#\\
# { 152 64 }#\\
# { 234 156 }#\\
#}
 
#</SCHEDULE>#\\ 

*/
//@{
 //@Include: convertgraph.hpp
 //@Include: irredtest.hpp
 //@Include: irrednode.hpp
 //@Include: graphalgo.hpp
 //@Include: partition.hpp
//@}
//******************************************************************************

int main(int argc, char *argv[]){

  G_ExceptionList.set_output_to_cout();   
  // open files and assure existence:
  if (argc != 3) {
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }
  
  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  
  if (!i_strm) {
    cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
    exit(1);
  }
  
  // read problem description and decide whether program is applicable:
  Lisa_ProblemType lpr;
  i_strm >> lpr;
  if (!G_ExceptionList.empty()){
    cout << "ERROR: cannot read problem type, aborting program." << endl;
    exit(1);
  }  
   
  
  // read control parameters: 
  int alg_param = Lisa_IrreducibilityTest::JUST_TEST;
  Lisa_ControlParameters sp;
  i_strm >> sp;
  if (sp.defined("TYPE")==STRING){
    if (sp.get_string("TYPE") == "ALL_REDUCING"){
      alg_param = Lisa_IrreducibilityTest::GENERATE_ALL;
    }else if(sp.get_string("TYPE") == "ITERATIVE_REDUCING"){
    }else if(sp.get_string("TYPE") == "SIMILAR"){
      alg_param = Lisa_IrreducibilityTest::GENERATE_SIMILAR;
    }else{
      cout << "WARNING: TYPE value out of Range, using default." << endl;
    }
  }else cout << "WARNING: Could not read TYPE parameter, using default." << endl;
  
  bool return_all = 0;
  if (sp.defined("RETURN")==STRING){
    if (sp.get_string("RETURN") == "ALL"){
      return_all = 1;
    }else if(sp.get_string("RETURN") == "ONLY_IRREDUCIBLE"){
    }else{
      cout << "WARNING: RETURN value out of Range, using default." << endl;
    }
  }else cout << "WARNING: Could not read RETURN parameter, using default." << endl;
  if (alg_param==Lisa_IrreducibilityTest::GENERATE_SIMILAR) return_all = 1;

  if (sp.defined("RNDM")==STRING){
    if (sp.get_string("RNDM") == "YES"){ 
      if(alg_param==Lisa_IrreducibilityTest::JUST_TEST) alg_param = Lisa_IrreducibilityTest::JUST_TEST_RANDOM;
    }else if(sp.get_string("RNDM") == "NO"){
    }else{
      cout << "WARNING: RNDM value out of Range, using default." << endl;
    }
  }else cout << "WARNING: Could not read RNDM parameter, using default." << endl;
 
  // read problem instance
  Lisa_Values my_werte;
  i_strm >> my_werte;
  
  Lisa_Schedule my_schedule;
  i_strm >> my_schedule;
  delete my_schedule.CIJ;
  my_schedule.CIJ = 0;

  Lisa_ConvertGraph* convert = Lisa_ConvertGraph::make_object(&lpr,my_werte.SIJ,my_werte.MO);
  
  if (!G_ExceptionList.empty()) exit(1);
 
  int v = (*convert->get_disjkt()).get_knots();
  Lisa_Graph pg(v);
  convert->plan2graph(my_schedule.LR,&pg);
  
  /*
  cout << *my_schedule.LR;
  cout << *convert->get_disjkt();
  cout << pg;
  convert->graph2plan(&pg,my_schedule.LR);
  cout << *my_schedule.LR;
 */
  
  Lisa_IrredResult* res;
  if (alg_param==Lisa_IrreducibilityTest::GENERATE_SIMILAR){ 
    res=new Lisa_IrredResult(Lisa_IrredResult::NO_FILTER);
  }else if(return_all){ 
    res =new Lisa_IrredResult(Lisa_IrredResult::ALL);
  }else{ 
    res = new Lisa_IrredResult(Lisa_IrredResult::POTENTIALLY_IRREDUCIBLE);
  }

  Lisa_IrreducibilityTest my_test(convert->get_disjkt());
  my_test.set_output_to(res);
  bool irre = my_test.test(&pg,alg_param);
  
  if (irre && alg_param!=Lisa_IrreducibilityTest::GENERATE_SIMILAR){
    cout << "WARNING: Plan is irreducible ! No new plans returned." << endl;
  }else{
    if(alg_param==Lisa_IrreducibilityTest::JUST_TEST ||
       alg_param==Lisa_IrreducibilityTest::JUST_TEST_RANDOM){
      res->results->reset();
      do{
	Lisa_IrredNode* curr = res->results->get();
	if(curr->status == Lisa_IrredNode::UNKNOWN){
	  irre = my_test.test(curr->plangraph,curr->compgraph,alg_param);
	  if (irre) curr->status = Lisa_IrredNode::IRREDUCIBLE;
	  else curr->status = Lisa_IrredNode::NOT_IRREDUCIBLE;
	}
      }while(res->results->next());
    }else{
      if(!return_all){
	res->compare_all();
	res->delete_reducible();
	res->results->reset();
	do{
	  res->results->get()->status = Lisa_IrredNode::IRREDUCIBLE; 
	}while(res->results->next());
      }
    }

    if(alg_param==Lisa_IrreducibilityTest::GENERATE_SIMILAR){ 
      if(irre) cout << "WARNING: Plan is irreducible !" << endl;
      o_strm << my_schedule;
    }

    res->results->reset();
    do{
      if(res->results->get()->status == Lisa_IrredNode::IRREDUCIBLE){
	convert->graph2plan(res->results->get()->plangraph,my_schedule.LR);
	o_strm << my_schedule;
      }else if(return_all){
      	convert->graph2plan(res->results->get()->plangraph,my_schedule.LR);
	o_strm << my_schedule;
      }
    }while(res->results->next());
    //  cout << "WARNING: Plan is NOT irreducible ! " << count << " irreducible plan(s) found." <<endl;
  }
 
  delete convert;
  delete res;
  
}










