#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>

const int ALPHABET_NO_LETTERS = 26;
const int A_ASCII = 65;
enum Coder_Type{PLUGBOARD=0,REFLECTOR=1,ROTOR=2,ROTOR_CONFIG = 3};

/* error codes  */

#define INSUFFICIENT_NUMBER_OF_PARAMETERS		1
#define INVALID_INPUT_CHARACTER				2
#define INVALID_INDEX					3
#define NON_NUMERIC_CHARACTER				4
#define IMPOSSIBLE_PLUGBOARD_CONFIGURATION		5
#define INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS	6
#define INVALID_ROTOR_MAPPING				7
#define NO_ROTOR_STARTING_POSITION			8
#define INVALID_REFLECTOR_MAPPING			9
#define INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS	10
#define ERROR_OPENING_CONFIGURATION_FILE		11

#define NO_ERROR					0

class error{
 public:
  error(){}

  static void impossible_parameters(int code){
    std::cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?\n";
    if(code == 1) throw(INSUFFICIENT_NUMBER_OF_PARAMETERS);
  }

  static void invalid_input_char(char in){
    std::cerr << in << " is not a valid input character ";
    std::cerr << "(input characters must be upper case letters A-Z)!\n";
    throw(INVALID_INPUT_CHARACTER);
  }

  static void inv_index(Coder_Type type){
    if(type == PLUGBOARD) std::cerr << "Invalid Plugboard Index\n";
    if(type == REFLECTOR) std::cerr << "Invalid Reflector Index\n";
    if(type == ROTOR) std::cerr << "Invalid Rotor Index\n";
    if(type == ROTOR_CONFIG) std::cerr << "Invalid Rotor Position Index\n";
    throw(INVALID_INDEX);
  }

  static void non_num(const char* filename, int type){
    if(type == PLUGBOARD)
      std::cerr << "Non-numeric character in plugboard file " << filename << std::endl;
    if(type == REFLECTOR)
      std::cerr << "Non-numeric character in reflector file " << filename << std::endl;
    if(type == ROTOR)
      std::cerr << "Non-numeric character for mapping in rotor file " << filename << std::endl;
    if (type == ROTOR_CONFIG)
      std::cerr << "Non-numeric character in rotor positions file " << filename << std::endl;
    throw(NON_NUMERIC_CHARACTER);
  }

  static void imp_config(int* error_code, int next, int index, Coder_Type type){
    if(error_code[0] >= 0){
      std::cerr << "Invalid mapping of input " << next << " to output ";
      std::cerr << index << " (output " << index;
      std::cerr << " is already mapped to from input " << error_code[0] << ")\n";
    }
    if(error_code[1] > 0){
      std::cerr << "Invalid mapping of input " << index << " to output ";
      std::cerr << next << " (output " << next;
      std::cerr << " is already mapped to from input " << error_code[1] <<")\n";
    }
    if(type == PLUGBOARD) throw(IMPOSSIBLE_PLUGBOARD_CONFIGURATION);
    if(type == REFLECTOR) throw(INVALID_REFLECTOR_MAPPING);
    if(type == ROTOR) throw(INVALID_ROTOR_MAPPING);
  }

  static void map_to_self(const char* filename, int error,Coder_Type type){
    std::cerr << "Cannot map character to itself: ";
    std::cerr << error << "in " << filename << std::endl;
    if(type==PLUGBOARD) throw(INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS);
    if(type== REFLECTOR) throw(INVALID_REFLECTOR_MAPPING);
    throw(IMPOSSIBLE_PLUGBOARD_CONFIGURATION);
  }

  static void rotor_incorrect_param(const char* filename){
    std::cerr << "Not all inputs mapped in rotor file: " << filename << std::endl;
    throw(INVALID_ROTOR_MAPPING);
  }

  static void rotor_config_toofew_param(int arg_diff,const char* filename){
    std::cerr << "No starting position for rotor";
      for(int i=0;i<arg_diff;i++)
	std::cerr << " " << i;
    std::cerr<< " in rotor position file: " << filename << std::endl;
    throw(NO_ROTOR_STARTING_POSITION);
  }

  static void rotor_config_toomany_param(const char* filename){
    std::cerr << "Too many parameters in rotor position file: " << filename << std::endl;
  }
  
  static void incorrect_param(const char* filename,Coder_Type type){
    if(type == REFLECTOR){
      std::cerr << "Insufficient number of mappings in reflector file: " << filename << std::endl;
      throw(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
    }
    return;
  }
  

  static void odd_param(const char* filename, Coder_Type type){  

    if(type == PLUGBOARD){
      std::cerr << "Incorrect number of parameters in plugboard file " << filename << std::endl;
      throw(INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS);
    }

    if(type == REFLECTOR){
      std::cerr << "Incorrect (odd) number of parameters in reflector file " << filename << std::endl;
      throw(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
    }
  }

  static void instream(Coder_Type type){
    if(type==PLUGBOARD)std::cerr << "Plugboard file cannot be opened\n";
    if(type==REFLECTOR)std::cerr << "Reflector file cannot be opened\n";
    if(type==ROTOR) std::cerr << "Rotor file cannot be opened\n";
    if(type==ROTOR_CONFIG)std::cerr << "Rotor positioning file cannot be opened\n";
    throw(ERROR_OPENING_CONFIGURATION_FILE);
  }

  
};


#endif
