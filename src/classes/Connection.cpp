#include "Connection.h"

Connection::Connection(std::string presyn, std::string postsyn, int w)
: presynaptic(presyn), postsynaptic(postsyn), weight(w) 
{
  std::ostringstream outStr;
  outStr << "Connection constructed: " << presynaptic << " | " 
         << postsynaptic << " | " << weight;
  logLine(LOGFILE, outStr.str(), false);
}
Connection::~Connection() = default;