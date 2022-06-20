#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <vector>
#include <chrono>
#include "../globals.h"
#include "../functions/declarations.h"
#include <sstream>
#include "Connection.h"

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using NanoSecDuration = std::chrono::nanoseconds;

class Connection;
class Neuron {
  friend class Stimulator;
  friend class Timer;
  friend class Sensor;
  //friend class Sensor;
  public:
    Neuron();    
    Neuron(
      std::string name, 
      int id, std::vector<int> postsyn, 
      std::vector<Connection*> connectome
    );    
    ~Neuron();

    void ping();//for testing
    void operator()();
    void feed_target(Neuron* n);
    std::string_view get_name();

  protected:
    int m_id {0};
    std::string m_name;
    std::vector<int> m_postsyn_connections;
    std::vector<Connection*> m_connectome;
    int m_accumulation_counter {0};
    TimePoint start_time;
    NanoSecDuration ellapsed_time;
    std::vector<Neuron*> targets;

    virtual void axon_fire();
    virtual void get_input(int weight);
    virtual void depolarise();
};//Neuron

#endif //NEURON_H