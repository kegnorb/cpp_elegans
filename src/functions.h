/*////////////////////////////////////////////////////////////
    Project: C. Elegans Connectome Emulation
    File: functions.h
    Version: 1.2.1 /2022/
    Content: Function Declarations and short descriptions
    Purpose: 
      - Simple logging 
      - Shell prompt
    Author: Norbert Keglevich
///////////////////////////////////////////////////////////*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "globals.h"
#include <unistd.h>
#include "classes.h"
#include <iostream>
#include <fstream>
#include <vector>

class Stimulator; 




/*********************************************************************************
    - Writes lines into a log file and optionally to console
    - @param file: takes a reference of the log file handler
    - @param msg: takes a string to forward it to the output stream(s)
    - @pram print: if true std::cout is active 
                  (besides writing in the log file)
*********************************************************************************/
void logLine(std::fstream& file, std::string msg, bool print);




/*********************************************************************************
    - Gives a (cli) shell prompt to initiate commands for the connectome
    - It is used as a two state (on/off) stimuli source for the various sensors 
    - For more information see the 'README.md' file
    - @param stimulators: takes a vector of Stimulator object pointers
                          these Stimulators will be activated or deactivated by
                          the user, giving the corresponding commands
    - @param log: takes a reference of the log file handler for logging
**********************************************************************************/
void stimulator_shell(std::vector<Stimulator*> stimulators, std::fstream& log);

#endif
