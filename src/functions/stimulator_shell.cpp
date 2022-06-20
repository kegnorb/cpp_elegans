#include "declarations.h"


void stimulator_shell(std::vector<Stimulator*> stimulators, std::fstream& log){
  std::string command;
  std::string separator {" "};
  std::vector<std::string> cmd_keywords;

  std::cout << "Type in 'help' for more info\n";
  char c;
  size_t pos{0};
  sleep(.5);
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

    for(auto kw : cmd_keywords) {
      std::cout << kw << "\n";
    }

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
            << "Chemosensory: 2\n"
            << "Anterior Touch: 3\n"
            << "Posterior Touch: 4\n";
    }
    else if (!cmd_keywords[0].compare("exit")) {
      RUN = false;
      break;
      
    }
    else {
      std::cout << "No such command " << command << "\n";
    }    
  }
  
  logLine(log, "Shell closed. Press Ctrl+C to terminate the program\n", true);
}//stimulator_shell
