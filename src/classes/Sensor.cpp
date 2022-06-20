#include "Sensor.h"

Sensor::Sensor(
  std::string_view name, 
  int id,  std::vector<int> postsyn, 
  std::vector<Connection*> connectome
  )
{
  m_name = name;
  m_id = id;
  m_postsyn_connections = postsyn;
  m_connectome = connectome;
  m_activated = false;
  std::cout << "Constructing: " << m_name << " : " << m_id<<std::endl;
  /*std::cout << "Postsyn connection ids: ";
  for (auto ps : postsyn) {
    std::cout << ps << ", ";
  }
  std::cout << "\n";*/
}    
void Sensor::activate() {
  m_activated = true;
}
void Sensor::deactivate() {
  m_activated = false;
}

void Sensor::get_input(int idle) {
  if(!idle && m_activated) { //idle == 0 esetén lesz true! Timer get_input(0)-val signaloz
    std::cout << m_name << " sensor fires\n";
    axon_fire();
  }
}

void depolarise() {
      //no depolarization
}
