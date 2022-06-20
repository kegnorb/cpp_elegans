#include "Stimulator.h"

Stimulator::Stimulator(std::string_view name, int id, Neuron* target_sensor){
  m_name = name;
  m_id = id;
  m_target_sensor = target_sensor;
  m_stimuli = "0";
}

void Stimulator::axon_fire() {
  dynamic_cast<Sensor*>(m_target_sensor)->activate();
}

void Stimulator::sensory_deprivation() {
  dynamic_cast<Sensor*>(m_target_sensor)->deactivate();
}