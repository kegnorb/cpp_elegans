/*//////////////////////////////////////////////////////
    Project: C. Elegans Connectome Emulation
    File: functions.cpp
    Version: 1.2.1 /2022/
    Content: Function Implementations
    Purpose: 
      - Simple logging 
      - Shell prompt
    Author: Norbert Keglevich
///////////////////////////////////////////////////////*/

#include "functions.h"




/**************************************************************************
    - Implementation of the simple logger
    - Logs are always written in a file regardless of @print value
    - For more information see the short description in the 'functions.h'
***************************************************************************/
void logLine(std::fstream& file, std::string msg, bool print) {
    if(print) {
        std::cout << msg << "\n";
    }

    file << msg << "\n";
}




/****************************************************************************
    - Implementation of the shell command line parser
    - Commands "transform" into Stimulator object handling operations
    - For more information see the short description in the 'functions.h'
    - or the 'README.md'
*****************************************************************************/
void stimulator_shell(std::vector<Stimulator*> stimulators, std::fstream& log){
    std::string command;
    std::string separator {" "};
    std::vector<std::string> cmd_keywords;

    std::cout << "Type in 'help' for more info\n";
    char c;
    size_t pos{0};
    sleep(.5); //NOTE: Avoid to get the first prompt printing mixed with the possible, lagging neuron intialization output
    while(RUN) {
        std::cout << "> ";
        std::getline(std::cin, command);

        command += " ";
        pos = 0;
        cmd_keywords.clear();

        while((pos = command.find(separator)) != std::string::npos) {
            cmd_keywords.push_back(command.substr(0, pos));
            command.erase(0, pos+separator.length());
        }

        command.clear();


        /**********************
          COMMANDLINE PARSING
        **********************/
        if(!cmd_keywords[0].compare("sensor")){
            if(!cmd_keywords[1].compare("nose")) {
                if(!cmd_keywords[2].compare("start")){
                    std::cout << "nose stimulation started\n";
                    stimulators[0]->axon_fire();
                }
                else if(!cmd_keywords[2].compare("stop")) {
                    std::cout << "nose stimulation stopped\n";
                    stimulators[0]->sensory_deprivation();
                }
                else {
                    std::cout << "no such option" << cmd_keywords[2] 
                              << " try 'start' or 'stop' instead\n";
                }
            }
            else if(!cmd_keywords[1].compare("chemo")) {
                if(!cmd_keywords[2].compare("start")){
                    std::cout << "chemosensory stimulation started\n";
                    stimulators[1]->axon_fire();
                }
                else if(!cmd_keywords[2].compare("stop")) {
                    std::cout << "chemosensory stimulation stopped\n";
                    stimulators[1]->sensory_deprivation();
                }
                else {
                    std::cout << "no such option" << cmd_keywords[2] 
                    << " try 'start' or 'stop' instead\n";
                }
            }
            else if(!cmd_keywords[1].compare("anterior")) {
                if(!cmd_keywords[2].compare("start")){
                    std::cout << "anterior stimulation started\n";
                    stimulators[2]->axon_fire();
                }
                else if(!cmd_keywords[2].compare("stop")) {
                    std::cout << "anterior stimulation stopped\n";
                    stimulators[2]->sensory_deprivation();
                }
                else {
                    std::cout << "no such option" << cmd_keywords[2] 
                              << " try 'start' or 'stop' instead\n";
                }
            }
            else if(!cmd_keywords[1].compare("posterior")) {
                if(!cmd_keywords[2].compare("start")){
                    std::cout << "posterior stimulation started\n";
                    stimulators[3]->axon_fire();
                }
                else if(!cmd_keywords[2].compare("stop")) {
                    std::cout << "posterior stimulation stopped\n";
                    stimulators[3]->sensory_deprivation();
                }
                else {
                    std::cout << "no such option" << cmd_keywords[2] 
                              << " try 'start' or 'stop' instead\n";
                }
            }
            else {
                std::cout << "No such sensor as " << cmd_keywords[1] 
                << ", try instead: 'nose', 'chemo' , 'anterior', 'posterior'\n";
            }
        }
        else if(!cmd_keywords[0].compare("help")) {
            std::cout << "Nose Touch: sensor nose start\n"
                      << "Chemosensory: sensor chemo start\n"
                      << "Anterior Touch: sensor anterior start\n"
                      << "Posterior Touch: sensor posterior start\n"
                      << "To Stop either of them, replace 'start with 'stop'\n"
                      << "For exiting the program type in: 'exit' or 'kill' or 'quit'\n";
        }
        else if (!cmd_keywords[0].compare("exit") 
                  || !cmd_keywords[0].compare("kill") 
                  || !cmd_keywords[0].compare("quit")
                 ) 
        {
            RUN = false;
            break;      
        }
        else {
            std::cout << "No such command " << command << "\n";
        }    
    }
  
    logLine(log, "Shell closed. Press Ctrl+C if terminated improperly\n", true);
}
