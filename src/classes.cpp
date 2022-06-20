/*//////////////////////////////////////////////////////////////////////////////////
    Project: C. Elegans Connectome Emulation
    File: classes.cpp
    Version: 1.2.1 /2022/
    Content: Class Implementations and short descriptions
    Purpose: 
      - Providing custom type "blueprints" for connectome components 
        and fundamental information storages for its working
        such as different kind of neurons, timers and connection descriptors
      - For more information check 'classes.h' and 'README.md'
    Author: Norbert Keglevich
//////////////////////////////////////////////////////////////////////////////////*/

#include "classes.h"





/**************************************************************************
    - Implements Connection entry
    - Basically a data struct with a constructor for easy initialization
**************************************************************************/
Connection::Connection(std::string presyn, std::string postsyn, int w)
: presynaptic(presyn), postsynaptic(postsyn), weight(w) 
{
    std::ostringstream outStr;
    outStr << "Connection constructed: " << presynaptic << " | " 
           << postsynaptic << " | " << weight;
    logLine(LOGFILE, outStr.str(), false);
}


Connection::~Connection() = default;





/*********************************************************************************
    - Implements CNS (Brain) Neuron type and its functionality
    - Also used as a base class for other derived types seen below
    - Instances run as independent threads 
    - operator () overloading implements the timer counter for depolarisation 
    -             and makes it possible to run as a thread
    - axon_fire() and receive_input() implements the I/O interface of the neuron
**********************************************************************************/
Neuron::Neuron() = default;
    

Neuron::Neuron(
    std::string name, 
    int id, std::vector<int> postsyn, 
    std::vector<Connection*> connectome
  ):
     m_name(name), 
     m_id(id), 
     m_postsyn_connections(postsyn), 
     m_connectome(connectome)
{
}


Neuron::~Neuron() {
    std::cout << m_name << " destructed\n";
}


void Neuron::ping() {
    std::cout << m_name << " : " << m_id 
              << " is working just fine.\n";
}


void Neuron::operator()(){
    std::cout << m_name << " : " << m_id << " neuron is up\n";

    auto start_time = std::chrono::steady_clock::now();

    while(RUN) {        
        ellapsed_time = std::chrono::steady_clock::now() - start_time;

        if(TO_MILLISEC(ellapsed_time).count() >= DEPOL_TIME) {
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


void Neuron::axon_fire() {
    std::cout << "Firing axon of neuron: " << m_name << "\n";

    m_accumulation_counter = 0;
    size_t i = 0;

    for(auto t : targets) {
        t->receive_input(m_connectome[m_postsyn_connections[i]]->weight);
        i++;
    }  
}


void Neuron::receive_input(int weight) {
    m_accumulation_counter += weight;

    if (m_accumulation_counter > CNS_TRESHOLD) {
        axon_fire();
    }
}


void Neuron::depolarise() {
    m_accumulation_counter = 0;
}





/***************************************************************************
    - Implements Sensor neuron type and its functionality
    - Input interface is extended with activate() and deactivate() methods
    - receive_input() expects only a special value (0) value from a Timer
************************************************************************** */
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
}    


void Sensor::activate() {
    m_activated = true;
}


void Sensor::deactivate() {
    m_activated = false;
}


void Sensor::receive_input(int idle) {
    if(!idle && m_activated) {
        std::cout << m_name << " sensor fires\n";
        axon_fire();
    }
}





/*******************************************************************************************************
    - Implements Motor neuron type and its functionality
    - receive_input() extended to receive special value of 0 identified as polling signal from a Timer
    - External (independent) Timer controlls the depolarisation mechanism
*******************************************************************************************************/
MotorNeuron::MotorNeuron(std::string_view name, int id )
{
    m_name = name;
    m_id = id;
}


void MotorNeuron::receive_input(int weight) {      
    if(weight == 0) {
        ellapsed_time = std::chrono::steady_clock::now() - timestamp;

        if (TO_MILLISEC(ellapsed_time).count() >= MOTOR_TIME) {
            depolarise();
        }
    }
    else {
        m_accumulation_counter += weight;
        timestamp = std::chrono::steady_clock::now();

        std::cout << "MOTOR NEURON Accumulated weights: " << m_accumulation_counter 
                  << " of " << this->m_name << "\n";
        
    }
}





/*****************************************************************************************************
    - Introduces an artifical neuron concept as an intermediator between 
      external source of stimuli and the corresponding Sensor
    - Output interface is modified by using Sensor type's activate() and deactivate() methods
    - I/O interface implementation is modified for compatibility with Sensor receiving interface
    - Invoking axon_fire() activates the Sensor connected to the Stimulator instance
    - Invoking sensory_deprivation() deactivates the Sensor connected to the Stimulator instance
*****************************************************************************************************/
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





/***************************************************************************************
    - Implements basic timing functionality
    - Takes pointer vector of target neurons for polling them with a certain frequency
    - operator () overload makes possible to run as a thread
****************************************************************************************/
Timer::Timer(int treshold_time, std::vector<Neuron*> t_neurons) 
: treshold_ms(treshold_time), target_neurons(t_neurons)
{      
}


void Timer::operator()(){
    start_time = std::chrono::steady_clock::now();
    ellapsed_time = std::chrono::steady_clock::now() - start_time;
    while(RUN){
        ellapsed_time = std::chrono::steady_clock::now() - start_time;

        if(TO_MILLISEC(ellapsed_time).count() >= treshold_ms) {
            for(auto t : target_neurons) {
                t->receive_input(0);
            }

            start_time = std::chrono::steady_clock::now();
        }
    }
}
