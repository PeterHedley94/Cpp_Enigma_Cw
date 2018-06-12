#include <iostream>
#include "coders.h"
#include "errors.h"
#include <fstream>
#include <stdlib.h>

using namespace std;

//#**************************************************************

coders :: coders(const int matrix[ALPHABET_NO_LETTERS]){
  for (int i = 0; i<ALPHABET_NO_LETTERS;i++) transform[i] = matrix[i];
}

//Simple character transform right to left passing through coder
void coders :: matmul(int& character) const{
  character = transform[character];
}

//Simple character transform left to right  passing through coder
void coders ::  trmatmul(int& character) const{
  for(int i=0; i<ALPHABET_NO_LETTERS; i++){
    if(transform[i] == character){
      character = i;
      return;
    }
  }
}

//Get input matrix from the reflector file
void coders :: get_matrix(const char* filename,
			  int matrix[ALPHABET_NO_LETTERS],Coder_Type type){
  
  ifstream fin;
  int first,second,index=0,error_code[2] = {-1,-1};
  fin.open(filename);
  if(fin.fail()) error::instream(type);
  do{
    fin >> first;
    if(!(fin.eof())){
      if(fin.fail())error::non_num(filename,type);
      fin >> second;		
      index ++;
      if(fin.eof()) error::odd_param(filename,type);
      if(index > (ALPHABET_NO_LETTERS/2)) error::incorrect_param(filename,type);
      if(fin.fail()) error::non_num(filename,type);
      if(first>= ALPHABET_NO_LETTERS||first <0)error::inv_index(type);
      if(second>=ALPHABET_NO_LETTERS || second <0)error::inv_index(type);
      if (first == second) error::map_to_self(filename, first,type);
      if( !(assign_to_matrix(first,matrix,second,error_code,false)) )
	error::imp_config(error_code,first,second,type);
      if( !(assign_to_matrix(second,matrix,first,error_code,false)) )
	error::imp_config(error_code,first,second,type);
    }
  }while(fin.good());

  if(index != (ALPHABET_NO_LETTERS/2)) error::incorrect_param(filename,type);
  fin.close();
}

//#***************************************************************

plugboard :: plugboard(const int matrix[]): coders(matrix){
}

reflector :: reflector(const int matrix[]): coders(matrix){
}

//***************************************************************

rotor::rotor(int setting,const int matrix[],const int notches[]):coders(matrix){
  for (int i = 0; i<ALPHABET_NO_LETTERS;i++) shift_notches[i] = notches[i];
  shift = 0;
  update_setting(setting);
}

//Get input matrix and rotor positions from the rotor file
void rotor :: get_matrix(const char* filename,int matrix[ALPHABET_NO_LETTERS],
			 int notches[ALPHABET_NO_LETTERS], Coder_Type type){
  
  ifstream fin;
  int error_code[2] = {-1,-1};
  fin.open(filename);
  if (fin.fail()) error::instream(type);
  
  int next, index=0;
 
  do{
    if(!(fin >> next) && !(fin.eof())) error::non_num(filename,type);
    if(index<ALPHABET_NO_LETTERS && fin.eof())
      error::rotor_incorrect_param(filename);
    if(next >= ALPHABET_NO_LETTERS || next <0) error::inv_index(type);
    
    if(index < ALPHABET_NO_LETTERS &&
       !(assign_to_matrix(next,matrix,index,error_code,true))){
      error::imp_config(error_code,next,index,type);
    }
    
    if(index>= ALPHABET_NO_LETTERS) notches[next] = 1;
    index ++;
  }while(fin.good());

  fin.close();
}


//Update the rotor positions and matrix
bool rotor :: update_setting(int shift_no){
  int temp_matrix[ALPHABET_NO_LETTERS]; int input; int value = 100;

  shift = shift + shift_no;

  for(int i =0 ; i <ALPHABET_NO_LETTERS ; i++){
    input = (i + shift_no)%ALPHABET_NO_LETTERS;
    value = transform[input];
    value = value - shift_no%ALPHABET_NO_LETTERS;
    if(value < 0) value = value + ALPHABET_NO_LETTERS;
    temp_matrix[i] = value;
  }
  for(int i=0; i<ALPHABET_NO_LETTERS; i++) transform[i] = temp_matrix[i];
  
  if(shift_notches[shift%ALPHABET_NO_LETTERS] == 1) return false;
  return true;  
}

//#*******************************************************************

// Assign value at position matrix[index][next] and checks for invalid entry
bool assign_to_matrix(int next,int matrix[ALPHABET_NO_LETTERS],
		      int index, int error_code[], bool rotor){
  
  if (matrix[index] > 0 && matrix[index] != index && rotor == false){
    error_code[0] = matrix[index];
    return false;
  }
  if (matrix[index] >= 0 && rotor == true){
    error_code[0] = matrix[index];
    return false;
  }
  for(int j; j<ALPHABET_NO_LETTERS; j++){
    if(matrix[j] == next && j != next){
	error_code[1] = j;
	return false;
      }
  }
  matrix[index] = next;
  return true;
}
