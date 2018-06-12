#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "coders.h"
#include "enigma.h"
#include "errors.h"
 
int main(int argc,const char** argv){
  
  try{
    enigma en;
    en.run_enigma(argc,argv,std::cin,std::cout);
  }catch (std::exception &exception){
    std::cerr << "Standard exception: " << exception.what() << '\n';
  }catch(int e){
    return e;
  }
  return 0;
}
