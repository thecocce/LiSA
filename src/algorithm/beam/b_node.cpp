#include "b_node.hpp"

B_Node::B_Node(B_Node *p):Lisa_OsSchedule(p->problem)
{
  
  ComputeHeadsTails( true, true);
  *this = *p;
  parent = p;
  cost = -1;
  this->lastAddedRow = p->lastAddedRow;
  this->lastAddedCol = p->lastAddedCol;
}
   
B_Node::B_Node(Lisa_OsProblem * p):Lisa_OsSchedule(p) {
  ComputeHeadsTails( true, true);
  parent = 0;
  cost = -1;
  problem = p;
}

int B_Node::insert(int i, int j, int k, int l) {
  this->lastAddedRow = i;
  this->lastAddedCol = j;
  this->cost = -1;
  int res = Lisa_OsSchedule::insert(i,j,k,l);
  return res;
}

TIMETYP B_Node::getCosts(){
  if (cost == -1) {
    SetValue(CMAX);
    TIMETYP v = GetValue();
    objective = (objective > v)?objective:v;
    if (costFunc == CMax)
      cost = GetValue();
    else
      cost = GetHead(lastAddedRow, lastAddedCol)+
	GetTail(lastAddedRow, lastAddedCol) +
	(*(P->time))[lastAddedRow][lastAddedCol];
  }
  return cost;
}

ostream & operator << ( ostream & os , B_Node & b){
  Lisa_Matrix<int> *lr = new Lisa_Matrix<int>(b.problem->n,b.problem->m);
  b.write_LR(lr);
  return os << *lr << endl;
} 



KList::KList(int k) {
  list = new (B_Node*)[k];
  in_list = 0;
  worst_in_list = -1;
  this->k = k;
}


void KList::add(B_Node *n) 
{
  if(in_list < k)
    {
      list[in_list]= n;
      if((worst_in_list < 0) || (n->getCosts() > list[worst_in_list]->getCosts()))
	worst_in_list = in_list;
      in_list++;
      return;
    }
  if(n->getCosts() >=list[worst_in_list]->getCosts()) {
    delete n;
    return;
  }
  delete list[worst_in_list];
  list[worst_in_list] = n;
  for(int i = 0; i<k;i++)
    if(list[worst_in_list]->getCosts() < list[i]->getCosts())
      worst_in_list = i;
}

void KList::add(B_Node *n,  int pos) 
{
  if (in_list > pos)
  {
    if (list[pos]->getCosts() > n->getCosts()) {
      delete list[pos];
      list[pos] = n;
    } else {
      delete n;
    }
  } 
  else {
    list[in_list] = n;
    in_list++;
  }
}

KList::~KList() {
  for (int i= 0; i < in_list; i++)
    delete list[i];
  delete list;
}