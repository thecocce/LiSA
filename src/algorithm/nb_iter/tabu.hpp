/*
 * ******************** tabu.h ******************************
 * 
 * description:      general problem independent tabulist class
 * 
 * owner:            Andreas Winkler
 *
 * date:             20.10.1998
 *
 */


#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _tabu_h
#define _tabu_h

#include <ostream.h>
#include "../../main/global.hpp"
#include "../../basics/matrix.hpp"

/** @name Tabu List    
*/
//@{
/** Problem Independent Tabulist.\\
    
    This tabu list is used with all of LiSA's neighbourhoods. You may also 
    use it to implement your own neighbourhood for use with LiSA's neighbourhood
    algorithms.\\
    
    There are two ways to use this object. You can either create a list with a given length. 
    In that case every entry in the tabu list contains 4 integers. You will have to use the functions
    #use()# and #set()# then. Otherwise you can create a list with a given length AND an entry length. 
    In that case any entry contains as much integers as you have specified. Entrys are then given as 
    vectors and you will have to use the functions #use_vector()# and #set_vector()#.\\
    
    This is also an example of how to implement your own tabu list.\\

    @see Lisa_Neighborhood
    @see Lisa_Iterator
    @author Andreas Winkler
    @version 2.3pre1
*/  
class Lisa_Tabu{
  
  Lisa_Vector<int> *tabu_1;     // tabulist
  Lisa_Vector<int> *tabu_2;     //
  Lisa_Vector<int> *tabu_3;     //
  Lisa_Vector<int> *tabu_4;     //
  Lisa_Matrix<int> *tabu;       // tabulist of vector
  int pos;
  unsigned int length, vector_length;
  
public:      
  /** constructor ...\\

      Construct the tabulist for entrys of 4 integers
      with specified list length. */
  Lisa_Tabu(unsigned int list_length);
  
  /** constructor ...\\

      Construct the tabulist for entrys of vectors
      with a specified list length and entry length. */
  Lisa_Tabu(unsigned int list_length, unsigned int entry_length);

  /** destructor ...\\
      
      Delete this object. */
  ~Lisa_Tabu();

  
  /** put an entry into the list ...\\
      
      Set an entry of 4 integers into the tabulist (returns #OK#). */
  int set( int, int, int, int );
  
  /** put an entry into the list ...\\

      Set a vector into the tabulist (returns #OK#). */
  int set_vector( Lisa_Vector<int>* );
  
  /** check whether an entry is in the list ...\\
      
      Search for an entry of 4 integers in the tabulist.
      #OK# is returned if that entry WAS NOT found and #!OK# if 
      that entry WAS found. */
  int use(int entry1, int entry2, int entry3, int entry4);

  /** check whether an entry is in the list ...\\
      
      Search for a vector in the tabulist. 
      #OK# is returned if that entry WAS NOT found and #!OK#
      if that entry WAS found. */
  int use_vector(Lisa_Vector<int>* entry);
};

#endif
//@}














