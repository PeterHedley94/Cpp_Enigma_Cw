#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include "errors.h"
#include "coders.h"
#include "enigma.h"

using namespace std;

enigma :: enigma(){
  identity(PLUGBOARD);
}

enigma :: ~enigma(){
  for(unsigned int i=0;i<coder_array.size();i++)
    delete coder_array[i];
}

//Convert transform matrix -> identity and notches to {0} for coder creation
void enigma :: identity(Coder_Type type){
  for(int m=0;m<ALPHABET_NO_LETTERS;m++){
    if(type == PLUGBOARD || type == REFLECTOR) transform[m] = m;
    if(type == ROTOR) transform[m] = -1;
    notches[m] = 0;
  }
}

//main function creates coders, loads text and runs program
void enigma :: run_enigma(int argc,const char** argv,istream& in, ostream& out){
  in.setf(ios::skipws);
  coder_array.reserve(10);
  
  if(argc == 4) error::impossible_parameters(0);
  if(argc<3) error::impossible_parameters(1);
  coders::get_matrix(argv[1],transform,PLUGBOARD);
  coder_array.push_back(new plugboard(transform));
  
  create_coders(argc,argv);
  character = get_character(in);
  
  while(character >= 0){
    character = character - A_ASCII;
    if(coder_array_size > 2) update_coders();
    walk_coders(character);
    out.put(static_cast<char>(character+A_ASCII));
    character = get_character(in);    
  }
}

//Get input character from instream.
int enigma :: get_character(istream& in){
  char input_char;
  in >> input_char;
  if(in.eof()) return -1;
  if(!isalpha(input_char) || !isupper(input_char) || !in.good()){
    error::invalid_input_char(input_char);
  }
  return static_cast<int>(input_char);
}

//Add the rotors, and reflector to vector if they have been specified.
// input of integer - number of specified files +1, and string of them + main
void enigma :: create_coders(int argc,const char** argv){

  identity(REFLECTOR);
  coders::get_matrix(argv[2],transform,REFLECTOR);
  coder_ptr rf = new reflector(transform);
  
  if(argc-4 > 0){
    
    for (int i = argc-4; i >= 1 ; i--){
      identity(ROTOR);
      rotor::get_matrix(argv[2+i],transform,notches,ROTOR);
      coder_array.push_back(new rotor(0,transform,notches));
    }

    int* number_ptr = new int[argc-4];
    check_input(argc,argv,number_ptr, ROTOR_CONFIG);
    coder_array_size = coder_array.size();
    
    for(int i = 1 ; i< coder_array_size;i++){
      coder_array[i]->update_setting(number_ptr[coder_array_size-i-1]);
    }
    delete[] number_ptr;
  }
  coder_array.push_back(rf);
  coder_array_size = coder_array.size();
}

//Check the validity of rotor position file - add positions to array number_ptr
//input parameters of integer no inputs, string of those, array number_ptr
void enigma::check_input(int argc,const char** argv,
			 int* number_ptr, Coder_Type type){
  ifstream fin;
  int next,index=0;

  fin.open(argv[argc-1]);
  if (fin.fail()) error::instream(type);
  
  do{
    if(!(fin >> next) && !(fin.eof())) error::non_num(argv[argc-1],type);
    if(next >= ALPHABET_NO_LETTERS || next <0) error::inv_index(type);
    if(index <= argc-4) number_ptr[index] = next;
    index ++;
  }while(fin.good());
  fin.close();
  if(index<argc-3) error::rotor_config_toofew_param((argc-3)-index,argv[argc-1]);
  if(index>argc-3) error::rotor_config_toomany_param(argv[argc-1]);
}

//Walk coders and modify input parameter int character through the coders
void enigma :: walk_coders(int& character) const{
  for(int i = 0; i<coder_array_size;i++) coder_array[i]->matmul(character);	    
  for(int i = coder_array_size-2; i>=0;i--) coder_array[i]->trmatmul(character);
}

//Update the rotor positions
void enigma :: update_coders() const{
  for(int i = 1 ; i<coder_array_size-1;i++){
    if(coder_array[i]->update_setting(1)) return;
  }
}
