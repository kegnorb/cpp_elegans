#ifndef GLOBALS_H
#define GLOBALS_H

#include <fstream>

#define TO_MILLISEC std::chrono::duration_cast<std::chrono::milliseconds>

const int TRESHOLD {15};
const int DEPOL_TIME {200};
const int SENSOR_POLL {100}; 
const int MOTOR_TIME {1000};

inline bool RUN {true};
inline std::fstream LOGFILE;
#endif