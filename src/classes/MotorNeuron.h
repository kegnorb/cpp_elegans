#ifndef MOTOR_NEURON_H
#define MOTOR_NEURON_H

#include <iostream>
#include <vector>
#include <chrono>
#include "../globals.h"
#include "../functions/declarations.h"
#include <sstream>
#include "Neuron.h"

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using NanoSecDuration = std::chrono::nanoseconds;

class MotorNeuron : public Neuron {
  friend class Timer;
  public:
    MotorNeuron(std::string_view name, int id );
    
  private:
    TimePoint timestamp; //store the last time it was stimulated
    NanoSecDuration ellapsed_time;

    virtual void get_input(int weight) override;    
    
};

#endif //CLASSES_H