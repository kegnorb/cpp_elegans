#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <iostream>

#include "../globals.h"
#include "../functions/declarations.h"
#include <sstream>



struct Connection {
  Connection(std::string presyn, std::string postsyn, int w); 
  ~Connection();

  std::string presynaptic;
  std::string postsynaptic;
  int weight;
};//Connection







#endif //CONNECTION_H