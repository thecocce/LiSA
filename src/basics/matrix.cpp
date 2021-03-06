/**
 * @author  Thomas Tautenhahn 21.10.99
 * @version 3.0
 */

#include <string.h>
#include <string>

#include "../main/global.hpp"
#include "matrix.hpp"
#include "pair.hpp"

using namespace std;

//**************************************************************************

template<class T>
Lisa_Vector<T>::Lisa_Vector(){
    m=0;
    contents=0;
}

//**************************************************************************

template<class T>
Lisa_Vector<T>::Lisa_Vector(const unsigned int m_in):m(m_in)
     {
       contents= new T[m];
     }  

//**************************************************************************

template<class T>
Lisa_Vector<T>::Lisa_Vector(const Lisa_Vector<T>& other):m(other.m)
     {
       contents= new T[m];
       memcpy (contents, other.contents, m*sizeof(T));
     }

//**************************************************************************

template<class T>
void Lisa_Vector<T>::fill(T wert) 
     {
       unsigned int i;
       for(i=m; i--; ) contents[i]=wert;
     }

//**************************************************************************

template<class T>
unsigned int Lisa_Vector<T>::index_of_max() const
{
  unsigned int i,j;
  T value=contents[0];
  T * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (*go>value) { value=*go; i=j; }
  return i;
}

//**************************************************************************

template<class T>
unsigned int Lisa_Vector<T>::index_of_min() const 
{
  unsigned int i,j;
  T value=contents[0];
  T * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (*go<value) { value=*go; i=j; }
  return i;
}

//**************************************************************************

template<class T>
const Lisa_Vector<T>& Lisa_Vector<T>::operator=(const Lisa_Vector<T>& other) 
     {
         
       if (&other == this) return *this;
  
       if ( m!=other.m)
        {
          G_ExceptionList.lthrow("wrong format argument to vector.operator=");
          return *this;
        } 
    
      
        memcpy (contents, other.contents, m*sizeof(T)); 
         // T MUST BE A TYPE WITHOUT DYNAMIC MEMORY!
        return *this;
     }  

//**************************************************************************
    
template<class T>
void Lisa_Vector<T>::write(ostream& strm)const{
  strm << "{ ";
  for (unsigned int i=0 ; i<m ; i++){
    strm.width(3); // write matrices with small entries in a nice way ;)
    strm << contents[i] << " ";
  }
  strm << "} " << endl;
}

//**************************************************************************

template<class T>
void Lisa_Vector<T>::read(istream& strm){

  string S="";
  strm >> S; 
  if (S!="{"){ 
    G_ExceptionList.lthrow("'{' expected in Lisa_Vector::read(), found '"+S+"'.",
                           Lisa_ExceptionList::SYNTAX_ERROR);
    return;
  }
  
  for (unsigned int i=0;i<m;i++) strm >> contents[i];
  
  S="";
  strm >> S; 
  
  if (S!="}") G_ExceptionList.lthrow("'}' expected in Lisa_Vector::read(), found '"+S+"'.",
                                     Lisa_ExceptionList::SYNTAX_ERROR);
}

//**************************************************************************

template<class T>
Lisa_Vector<T>::~Lisa_Vector()
     {
#ifdef LISA_DEBUG
       if (!contents) G_ExceptionList.lthrow("vector without contents");
#endif       
       if(contents) delete[] contents;
     }

//**************************************************************************

template<class T>
bool Lisa_Vector<T>::operator==(const Lisa_Vector<T>& other) const
     { 
       unsigned int i;
       for (i=0; i<m; i++) 
          if (contents[i]!= other.contents[i]) return false;
       return true;
     }

//**************************************************************************

template<class T>
bool Lisa_Vector<T>::operator<=(const Lisa_Vector<T>& other) const
     { 
       unsigned int i;
       for (i=0; i<m; i++) 
          {
            if (contents[i]> other.contents[i]) return false;
            if (contents[i]< other.contents[i]) return true;
          }
       return true;
     }

//**************************************************************************

template<class T>
Lisa_Matrix<T>::Lisa_Matrix(const Lisa_Matrix<T>& other):m(other.m),n(other.n){
    
       row=new Lisa_Vector<T>[n];
       
       for(unsigned int i=n;i--;){
           row[i].m = m;
           row[i].contents = new T[m];
           row[i]=other.row[i]; 
       }
}

//**************************************************************************

template<class T>
Lisa_Matrix<T>::Lisa_Matrix(int n_in, int m_in):m(m_in),n(n_in){
       row=new Lisa_Vector<T>[n];
       
       for(unsigned int i=n;i--;){
           row[i].m = m;
           row[i].contents = new T[m];
       }
}

//**************************************************************************

template<class T>
void Lisa_Matrix<T>::fill(const T wert) 
     {
       int i;
       for ( i=n ; i-- ; )
           row[i].fill(wert);
     }


//**************************************************************************

template<class T>
const Lisa_Matrix<T>& Lisa_Matrix<T>::operator=(const Lisa_Matrix<T>& other) 
{
  Lisa_Vector<T> * rowptr;
  Lisa_Vector<T> * otherptr;
  Lisa_Vector<T> * endptr;

  if (&other == this) return *this;
  
  if ((n!=other.n) || (m!=other.m))
    {
      G_ExceptionList.lthrow("wrong format argument to matrix.operator=");
      return *this;
    } 

  endptr=end_row();
  for (rowptr=first_row(), otherptr=other.first_row(); rowptr!=endptr; 
       rowptr++, otherptr++)
    *rowptr=*otherptr; 
  return *this;
}      

//**************************************************************************

template<class T>
void Lisa_Matrix<T>::write(ostream& strm)const{

  strm << "{ " << endl;
  for (unsigned int i=0 ; i<n ; i++)  strm << " " << row[i];
  strm << "} " << endl;
}

//**************************************************************************

template<class T>
void Lisa_Matrix<T>::read(istream& strm){
 
  string S="";
  strm >> S; 
  if (S!="{"){
   G_ExceptionList.lthrow("'{' expected in Lisa_Matrix::read(), found '"+S+"'.",
                          Lisa_ExceptionList::SYNTAX_ERROR);
   return;
  }
  
  for(unsigned int i=0;i<n;i++) strm >> row[i];
    
  S="";
  strm >> S; 
  
  if (S!="}") G_ExceptionList.lthrow("'}' expected in Lisa_Matrix::read(), found '"+S+"'.",
                                     Lisa_ExceptionList::SYNTAX_ERROR); 
} 

//**************************************************************************

template<class T>
Lisa_Matrix<T>::~Lisa_Matrix()
     {
       if (row) delete[] row;
     }

//**************************************************************************

template<class T>
bool Lisa_Matrix<T>::operator==(const Lisa_Matrix<T>& other) const
     { 
       unsigned int i;
       for (i=0; i<n; i++) 
          if (!(row[i]== other.row[i])) return false;
       return true;
     }

//**************************************************************************

template<class T>
bool Lisa_Matrix<T>::operator<=(const Lisa_Matrix<T>& other) const
     { 
       unsigned int i, j;
       for (i=0; i<n; i++)
          for (j=0; j<m; j++) 
            {
              if (row[i][j]> other.row[i][j]) return false;
            if (row[i][j]< other.row[i][j]) return true;
          }
       return true;
     }


//**************************************************************************

// define classes for following types T:
template class Lisa_Matrix<bool>;
template class Lisa_Matrix<int>;
template class Lisa_Matrix<long>;
template class Lisa_Matrix<float>;
template class Lisa_Matrix<double>;
template class Lisa_Matrix<Lisa_Pair>;

template class Lisa_Vector<bool>;
template class Lisa_Vector<int>;
template class Lisa_Vector<long>;
template class Lisa_Vector<float>;
template class Lisa_Vector<double>;
template class Lisa_Vector<string>;
template class Lisa_Vector<Lisa_Pair>;

//**************************************************************************

