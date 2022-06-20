/*///////////////////////////////////////////////////////////////////////////
    Project: C. Elegans Connectome Emulation
    File: globals.h
    Version: 1.2.1 /2022/
    Content: Globally used macros, constants, variables
    Purpose: 
      - Storing macro(s) 
      - Storing the manually adjustable parameters for the connectome
      - Storing and initializing the variables with global accessibility
    Author: Norbert Keglevich
//////////////////////////////////////////////////////////////////////////*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <fstream>
#include <vector>

/*  "Shorthand" for millisecond conversion  */
#define TO_MILLISEC std::chrono::duration_cast<std::chrono::milliseconds>




/***********************************************************************************
    CNS_TRESHOLD:   Value when reached, the given CNS neuron fires
    DEPOL_TIME:     The period [ms] waited for the event 
                    to reach the TRESHOLD before getting depolarised anyway
    SENSOR_POLL:    The period [ms] for polling Sensor objects state
                    If a Sensor is active it will fire with that polling frequency
    MOTOR_TIME:     The period [ms] waited for a given MotorNeuron object 
                    to get stimulated (again) before getting depolarised    
    SENSOR_NAMES:   Stores the name of the Sensors that identifies them                   
***********************************************************************************/
const int CNS_TRESHOLD {15};
const int DEPOL_TIME {200};
const int SENSOR_POLL {100}; 
const int MOTOR_TIME {1000};

const std::vector<std::string> SENSOR_NAMES{
    "noseSens",
    "chemoSens",
    "anteriorSens",
    "posteriorSens"
};




/************************************************************************************
    RUN:            Variable to globally hold or end 
                    the while(RUN) loops of neurons and the main() function
                    It is responsible for controlling the program flow 
                    therefore ending the program properly 
    LOGFILE:        Variable to store the file handler of the log file (log.txt)
                 
*************************************************************************************/
inline bool RUN {true};
inline std::fstream LOGFILE;

#endif
