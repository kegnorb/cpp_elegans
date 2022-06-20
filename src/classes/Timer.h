#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <vector>
#include <chrono>
#include "../globals.h"
#include "../functions/declarations.h"
#include <sstream>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using NanoSecDuration = std::chrono::nanoseconds;

class Timer {
  public:
    Timer(int treshold_time, std::vector<Neuron*> t_neurons);
    void operator()();

  private:
    int treshold_ms;
    TimePoint start_time;
    NanoSecDuration ellapsed_time;
    std::vector<Neuron*> target_neurons;
};//Timer


#endif