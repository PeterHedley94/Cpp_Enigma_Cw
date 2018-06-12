#ifndef ENIGMA_H
#define ENIGMA_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include "errors.h"
#include "coders.h"


class enigma{
 public:
  enigma();
  ~enigma();

  //main function creates coders, loads text and runs program
  void run_enigma(int argc,const char** argv,std::istream& in, std::ostream& out);

  //Check the validity of rotor position file - add positions to array number_ptr
  void check_input(int argc,const char** argv,int* number_ptr,Coder_Type type);

  // Convert transform matrix to identity and notches to {0} used for coder creation
  void identity(Coder_Type type);
  
  //Add the rotors, and reflector to vector if they have been specified.
  void create_coders(int argc,const char** argv);

  //Walk coders and modify input parameter int character through the coders
  void walk_coders(int& character) const;

  //Update the rotor positions
  void update_coders() const;
  
  //Get input character from instream.
  int get_character(std::istream& in);

 private:
  std::vector<coder_ptr> coder_array;
  int coder_array_size;
  int transform[ALPHABET_NO_LETTERS];
  int notches[ALPHABET_NO_LETTERS];
  int character;
};




#endif
