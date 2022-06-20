#include "declarations.h"


void logLine(std::fstream& file, std::string msg, bool print) {
  if(print) {
    std::cout << msg << "\n";
  }
  file << msg << "\n";
}