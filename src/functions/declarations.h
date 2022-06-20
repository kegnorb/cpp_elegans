#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "../globals.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "../classes/Stimulator.h"

class Stimulator;
void logLine(std::fstream& file, std::string msg, bool print);
void stimulator_shell(std::vector<Stimulator*> stimulators, std::fstream& log);

#endif