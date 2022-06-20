/*///////////////////////////////////////////////////////////////////////////////
    Project: C. Elegans Connectome Emulation
    File: classes.h
    Version: 1.2.1 /2022/
    Content: Class Definitions and short descriptions
    Purpose: 
      - Class declarations/definitions
      - Providing custom type "blueprints" for connectome components 
        and fundamental information storages for its working
        such as different kind of neurons, timers and connection descriptors
    Author: Norbert Keglevich
///////////////////////////////////////////////////////////////////////////////*/

#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <vector>
#include <chrono>
#include "globals.h"
#include "functions.h"
#include <sstream>

/**************************************************
    Type aliases for std::chorno time utilities 
**************************************************/
using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using NanoSecDuration = std::chrono::nanoseconds;





/***************************************************************************************************************
    - Data struct for storing and initializing Connection objects
    - Fundamental building block of the connectome map
    - A Connection is defined by identifing:
        - presynaptic (AKA: sender) neuron - (the neuron participating with its axon in the connection)
        - postsynaptic (AKA: receiver) neuron - (the neuron participating with its dendrit in the connection)
        - weight - the value that the sender neuron sends to the receiver neuron
    - A connectome map is a vector (or array) of Connection objects which can be used for a given neuron 
      to identify its target (receiver) neurons and the corresponding weights to send them when it fires  
****************************************************************************************************************/
struct Connection {
    Connection(std::string presyn, std::string postsyn, int w); 
    ~Connection();

    std::string presynaptic;
    std::string postsynaptic;
    int weight;
};





/*****************************************************************************************************************
    - Class for modelling CNS (Central Nervous System - AKA: Brain) Neurons
    - Also a base class for more specific neurons such as MotorNeuron or Sensor classes
    - Constructor takes:
        @param name: The name of the Neuron instance (used for identification)
        @param id: The id (or NID) of the Neuron instance (no particular usage in this version)
                   It gives an other option for identification, maybe for later optimalizations and updates
        @param postsyn: Takes the Postsynaptic IDs that is later used for extracting the corresponding 
                        Connection entries (postsynaptic/target neuron names and weights) from the 
                        connectome map for the given Neuron instance when needed (mostly at firing)
                        The argument passed is basically a vector storing the indexes
                        of the connection map entries, belonging to the given Neuron instance
        @connectome: Takes a vector of pointers to the connectome map entries for easy access
    - operator () is overloaded - object can be used as a functor (for creating threads)
    - feed_target() can be used after initialization to push the pointers of the target neurons one by one
    - @member targets: the vector storing the taken pointers pointing to the target neurons
    - @member accumulation_counter: collects the incoming weights
    - @member start_time, ellapsed_time: used for timing the depolarisation
    - axon_fire() sends the corresponding weights to the target neurons (using their receive_input method)
    - receive_input() used for receiving input from other presynaptic neurons 
                  and add the incoming value to the accumulation_counter
    - depolarise() zeros out the accumulation_counter if the given period (DEPOL_TIME) ellapsed without 
                   accumulation_counter reaching the CNS_TRESHOLD value    
******************************************************************************************************************/
class Neuron {
    friend class Stimulator;
    friend class Timer;
    friend class Sensor;

    public:
        Neuron();    
        Neuron(
          std::string name, 
          int id,
          std::vector<int> postsyn, 
          std::vector<Connection*> connectome
        );    
        virtual ~Neuron();
    
        void ping();
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
        virtual void receive_input(int weight);
        virtual void depolarise();
};





/**********************************************************************************************
    - Class for modelling Sensors (e.g.: Nose, Food, Touch)
    - They can be modelled as a specific kind of Neuron receiving two state inputs from 
      external Stimulator objects 
    - @member m_activated: If true the Sensor breaks out of the idle state and
                           fires every time the corresponding independent Timer polls it 
    - activate() can be called by a Stimulator to set the m_activated member variable
    - deactivatte() can be called by a Stimulator to unset the m_activated member
**********************************************************************************************/
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
        
        virtual void receive_input(int idle) override;        
        virtual void depolarise() override {
          //NOTE: no depolarization
        }
};





/***************************************************************************************************
    - Models Motor Neurons
    - They can be modelled as a specific kind of Neuron those never fire and
      the depolarisation mechanism is controlled by an outside independent Timer object
    - @member timestamp: Stores the time of the last incoming weight value
                         If it is older than MOTOR_TIME the MotorNeuron instance will depolarise
    - receive_input() is modified the way that, if it receives 0 than it means the Timer object is
                  polling, every other value is regular incoming weight from presynaptic neurons    
***************************************************************************************************/
class MotorNeuron : public Neuron {
    friend class Timer;

    public:
        MotorNeuron(std::string_view name, int id );
      
    private:
        TimePoint timestamp; 
        NanoSecDuration ellapsed_time;
    
        virtual void receive_input(int weight) override;        
};





/****************************************************************************************************************
    - Stimulator can be thought as an artificial, specialized Neuron for controlling the Sensors
    - They can be controlled by outside sources (like various hardware units or even implicitly by a user)
    - In this version the user controls the Stimulators via a simple shell command line interface
    - axon_fire() sends activation signal for the corresponding Sensor instance (pointed by m_target_sensor)
    - sensory_deprivation() sends deactivation signal for corresponding Sensor    
****************************************************************************************************************/
class Stimulator : public Neuron{
    friend void stimulator_shell(std::vector<Stimulator*> stimulators, std::fstream& log);

    public:
        Stimulator(std::string_view name, int id, Neuron* target_sensor);
  
    private:
        Neuron* m_target_sensor;
        std::string m_stimuli;
    
        virtual void axon_fire() override; 
        void sensory_deprivation();
};





/*******************************************************************************************************************
    - Models Timer Counters
    - operator () is overloaded - can be used as functor (for creating threads)
                  contains the meat of the timer counter implementation
    - @members treshold_ms, start_time, ellapsed_time: helps to manage the timing functionality
    - @member target_neurons: Holds the pointers to the corresponding neurons to be polled by the Timer instance
                              Uses the receive_input() method of the target Neurons to send Timer signal 
                              Timer signal is a 0 value of "weight" (it is not gonna induce any accumulation)              
********************************************************************************************************************/
class Timer {
    public:
        Timer(int treshold_time, std::vector<Neuron*> t_neurons);
        void operator()();
  
    private:
        int treshold_ms;
        TimePoint start_time;
        NanoSecDuration ellapsed_time;
        std::vector<Neuron*> target_neurons;
};

#endif
