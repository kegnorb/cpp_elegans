#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <vector>
#include <chrono>
#include "../globals.h"
#include "../functions/declarations.h"
#include <sstream>
#include "Neuron.h"


class Connection;
class Neuron;
class Sensor : public Neuron {
  friend class Stimulator;
  public:
    Sensor(
      std::string_view name, 
      int id,  std::vector<int> postsyn, 
      std::vector<Connection*> connectome
    );

    void activate();
    void deactivate();
     
  private:
    bool m_activated;
    
    virtual void get_input(int idle) override;
    virtual void depolarise() override;
};//Sensor


#endif