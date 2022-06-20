#include "MotorNeuron.h"

MotorNeuron::MotorNeuron(std::string_view name, int id)
{
  m_name = name;
  m_id = id;
  std::cout << "Constructing: " << m_name << " : " << m_id << std::endl;
}

void MotorNeuron::get_input(int weight) {      
  if(weight == 0) {
    //std::cout << "Checking timestamp of " << m_name << "\n";
    //depolarise if now-timestamp > MOTOR_TIME
    ellapsed_time = std::chrono::steady_clock::now() - timestamp;
    if (TO_MILLISEC(ellapsed_time).count() >= MOTOR_TIME) 
    {
      depolarise();
    }
  }
  else {
    m_accumulation_counter += weight;
    std::cout << "MOTOR NEURON Accumulated weights: " << m_accumulation_counter 
    << " of " << this->m_name << "\n";
    timestamp = std::chrono::steady_clock::now();
  }
}