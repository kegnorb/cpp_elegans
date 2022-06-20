#include "Neuron.h"

Neuron::Neuron() = default;
    
Neuron::Neuron(
  std::string name, 
  int id, std::vector<int> postsyn, 
  std::vector<Connection*> connectome
)
  :m_name(name), 
   m_id(id), 
   m_postsyn_connections(postsyn), 
   m_connectome(connectome)
{
  /*std::cout << "Constructing: " << m_name << " : " << m_id<<std::endl;
  std::cout << "Postsyn connection ids: ";
  for (auto ps : postsyn) {
    std::cout << ps << ", ";
  }
  std::cout << "\n";*/  
}

Neuron::~Neuron() {
  std::cout << m_name << " destructed\n";
}

void Neuron::ping() {
  std::cout << m_name << " : " << m_id << " is ping..\n";
}

void Neuron::operator()(){
  std::cout << m_name << " : " << m_id << " neuron is up\n";
  auto start_time = std::chrono::steady_clock::now();
  //axon_fire(); //for dbg
  while(RUN) {        
    ellapsed_time = std::chrono::steady_clock::now() - start_time;
    if(TO_MILLISEC(ellapsed_time).count() >= DEPOL_TIME)
    {
      depolarise();
      start_time = std::chrono::steady_clock::now();
    }
    
  }
}

void Neuron::feed_target(Neuron* n) {
  targets.push_back(n);
}

std::string_view Neuron::get_name(){
  return m_name;
}

void Neuron::axon_fire() {//virtual!
  std::cout << "Firing axon of neuron: " << this->m_name << "\n";
  /*std::cout << "Targetting: ";
  for(auto psc : m_postsyn_connections) {
    std::cout << m_connectome[psc]->postsynaptic << " : " 
    << m_connectome[psc]->weight << ", ";
  }
  std::cout << "\n";*/
  m_accumulation_counter = 0;
  size_t i = 0;
  for(auto t : targets) {
    //t->ping();
    t->get_input(m_connectome[m_postsyn_connections[i]]->weight);
    i++;
  }  
}//axon_fire

void Neuron::get_input(int weight) {
  //std::cout << "Input weight: " << weight << " received in " << this->m_name << "\n";
  m_accumulation_counter += weight;
  //std::cout << "Accumulated weights: " << m_accumulation_counter 
  //<< " of " << this->m_name << "\n";
  if (m_accumulation_counter > TRESHOLD) {
    axon_fire();
  }
}

void Neuron::depolarise() {
  m_accumulation_counter = 0;
  //std::cout << m_name <<" depolarized\n";
}