#ifndef BEAM_SEARCH_HPP
#define BEAM_SEARCH_HPP

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../basics/pair.hpp"
#include "beam_orders.hpp"

class LisaXmlFile;

class BeamSearch {

public:
  explicit BeamSearch();
  
  typedef enum {
    insert1,
    insert2
  } InsertionMethod;

  typedef enum {
    CObjective,
    CLast
  }  CostFunc;
  
  typedef enum {
    machines,
    jobs,
    both
  } AttatchmentRule;

  int problemtype;
  int beam_width;
  InsertionMethod insertionMethod;
  InsertionOrder iord;
  AttatchmentRule attatch;
  CostFunc costFunc;
  int destObjective;
  
  bool init(LisaXmlFile& input);
  bool run();

  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;
  Lisa_Values Values;

  Lisa_OsProblem *problem;
  Lisa_Schedule * result;
  TIMETYP value;
  Lisa_Order *order;
  ~BeamSearch();
  TIMETYP guessObjective();

  int step, n_ops;

private:
  void clear();
  typedef std::pair<int,int> Operation;
  typedef std::pair<int,int> OpRankPos;
  typedef std::pair<Operation, OpRankPos> OpInsertion;
  typedef std::vector<OpInsertion> InsertionList;
  bool getNextOp(class B_Node* parent, BeamSearch::Operation& next);
  void getDescendentInsertions(class B_Node& parent, const Operation& op, InsertionList& ins);
  void getDescendentAppendings(class B_Node& parent, const Operation& op, InsertionList& ins);
  

};


#endif