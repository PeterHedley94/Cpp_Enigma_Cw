#ifndef CODERS_H
#define CODERS_H
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "errors.h"

class coders;
typedef coders* coder_ptr;

//#**************************************************************

class coders{
public:
  coders(const int matrix[]);
  virtual ~coders(){};
  //Simple character transform right to left passing through coder
  virtual void matmul(int& character) const;
  //Simple character transform left to right  passing through coder
  virtual void trmatmul(int& character) const;
  virtual bool update_setting(int shift_no)=0;
  //Get input matrix from the plugboard file
  static void get_matrix(const char* filename, int matrix[ALPHABET_NO_LETTERS], Coder_Type type);
protected:
  int transform[ALPHABET_NO_LETTERS];
  int setting;
};

//#**************************************************************

class plugboard : public coders{
public:
  plugboard(const int matrix[]);
  bool update_setting(int shift_no){return false;}
};

//#**************************************************************

class reflector : public coders{
public:
  reflector(const int matrix[]);
  bool update_setting(int shift_no){return false;}
};

//#**************************************************************

class rotor : public coders{
  int shift;
  int shift_notches[ALPHABET_NO_LETTERS];
public:
  rotor(int setting,const int matrix[],const int notches[]);
  
  //Get input matrix and rotor positions from the rotor file
  static void get_matrix(const char* filename, int matrix[ALPHABET_NO_LETTERS], int notches[ALPHABET_NO_LETTERS],Coder_Type type);

  //Update the rotor positions and matrix
  bool update_setting(int shift_no);
};

//#**************************************************************

// Assign value at position matrix[index][next] and checks for invalid entry
bool assign_to_matrix(int next,int matrix[ALPHABET_NO_LETTERS],int index, int error_code[], bool rotor);

#endif


