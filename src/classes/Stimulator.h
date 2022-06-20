#ifndef STIMULATOR_H
#define STIMULATOR_H

#include <iostream>
#include <vector>
#include <chrono>
#include "../globals.h"
#include "../functions/declarations.h"
#include <sstream>
#include "Neuron.h"
#include "Sensor.h"

class Neuron;
//Stimulator is only a Neuron like entity not a "real" Neuron in functionality
class Stimulator : public Neuron{
  friend void stimulator_shell(std::vector<Stimulator*> stimulators, std::fstream& log);
  public:
    Stimulator(std::string_view name, int id, Neuron* target_sensor);

  private:
    Neuron* m_target_sensor;
    std::string m_stimuli;

    virtual void axon_fire() override; 
    void sensory_deprivation();

};//Stimulator

#endif 