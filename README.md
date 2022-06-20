## Basic Information
- C. Elegans Connectome Simulation
- Programming language: C++20
- Version: 1.2.1
- Year: 2022
- Author: Norbert Keglevich

## About
- This project intended to simulate basic workings of the C. Elegans connectome and possibly attaching its I/O to hardware units hence representing basic movements to different stimuli of the real nematode
- This implementation is based on the early version of Tim Busbice's work, but it is not an exact replica rather a remake using C++ and threads instead of independent processes with UDP sockets
- The central nervous system of the C. Elegans (Caenorhabditis Elegans) has only about 300 neurons. The connections between them were fully mapped and the information is available for everyone
- The project's intent is to simulate every neuron individually (running independently or parallel) using multithreading with C++
- Obviously ordinary (even multicore) PCs are not designed for running so many time-critical processes and guarantee sufficient pseudo parallelism before running out of resources, therefore when evaluating the results of this approach this should be taken into consideration

## Overview of Technical Implementation
- The program uses various class object instances to represent the state and inner workings of neuron types concerning the highly simplified mathematical model of C. Elegans neurons
- In essence a neuron component is just an information processing unit, receiving, accumulating and sending weighted (int) values
- These neuron components are the basic building blocks of the connectome
- The CNS (or brain) neuron type (class) `()` operator is overloaded so it can be taken as an argument by `std::thread` threads
- The overloaded `()` operator implements the independent timer counter for depolarisation (every CNS (base class) `Neuron` instance will have their own timer counter when multithreading is used)
- For motor neurons and sensors an independent `Timer` object are used each. These timers poll these neurons to check if they were stimulated since the last time check
- `MotorNeuron` objects use timestamp to determine if depolarisation is needed
- A `Sensor` object uses a state member variable to determine itself as active or not (if active, every time the timer polls, it fires to postsynaptic target neurons belonging to that biological sensor it is modelling)
- `Stimulator` objects are not part of the real world C. Elegans nervous system of any way, rather are some abstraction introduced only by the program that provides neuron like behaviour on the output side - interfacing with `Sensor` instances - and an easily accessible interface on the input side for the developer to channel in the outside context, such as prepocessed hardware sensor outputs, or user commands. A `Stimalator` can be thought as a "switch" which allows or disallows the respective `Sensor` to fire
- In this version a rather simple CLI shell wraps around the Stimulator instances to connect them explicitly with the user generated inputs (commands) via terminal

## Instructions
### Database Layout
- cpp_elegans_db <pre>
+--------------------------+
| Tables_in_cpp_elegans_db |
+--------------------------+
| cns_neurons_tbl          |
| connectome_tbl           |
| motor_neurons_tbl        |
+--------------------------+</pre>
- Sample of the cns_neurons_tbl:
+------+------+
| name | nid  |
+------+------+
| ADAL |    0 |
| ADAR |    1 |
| ADEL |    2 |
| ADER |    3 |
| ADFL |    4 |
| ADFR |    5 |
+------+------+
- Sample of the motor_neurons_tbl:
+-------+------+
| name  | nid  |
+-------+------+
| MDL01 |  300 |
| MDL02 |  301 |
| MDL03 |  302 |
| MDL04 |  303 |
| MDL05 |  304 |
| MDL06 |  305 |
+-------+------+
- Sample of the connectome_tbl:
+-------------+--------------+--------+
| presynaptic | postsynaptic | weight |
+-------------+--------------+--------+
| ADAL        | ADAR         |      1 |
| ADAL        | ADFL         |      1 |
| ADAL        | AIBL         |      1 |
| ADAL        | AIBR         |      2 |
| ADAL        | ASHL         |      1 |
| ADAL        | AVAR         |      2 |
+-------------+--------------+--------+
- `name`, `presynaptic`, `postsynaptic` fields are `VARCHAR` type, while `nid` and `weight` are `INT`
### Setting up the database
- On Linux system after installing mysql (MariaDB): `mysql service start`
- Using SQL statments the database with the corresponding tables can be created beforehand, e.g.: `CREATE DATABASE cpp_elegans_db;`
- in this rudimentary version the credentials needed for accessing the database should be provided in the source code before compilation
### Compiling
- on Linux systems the compiler should be set up somewhat similar as shown below:
`g++ -I/usr/include/cppconn -o c_elegans.bin ./*.cpp -pthread -L/usr/lib -lmysqlcppconn`
- When using VS Code the task.json file should be set up respectively of the above parameters
- To get the mysql connector: `apt-get install libmysqlcppconn-dev`
- Include paths for mysql connector might need editing

## Known Issues and Deficiencies
- Thread destrucion and proper program termination is not fully implemented. The custom shell might not even responds after connectome activation
- No real parallism: System overload, laggings and freezes should be expected because of probable incapacity of the underlying hardware to deal with the amount of real time parallelism demanded by this connectome implementation
- Parameters such as `CNS_TRESHOLD` or `DEPOL_TIME` might be neccessary to fine tune manually
- Source code needs further refactoring
- Associative Container types should be considered to use when possible instead of vectors for better run-time performance when searching for connectome information
- Organising the source code into multiple files: Might every major class should have its own header and implementation file (.h, .cpp)
- A more user friendly GUI for controlling and showing feedback information is definitely missing and should be implemented in the near future
- Appropriate hardware units could also be introduced for extending the scope of C. Elegans behaviour representation. That way the resulted movement patterns by external stimuli - for which presumably the connectome alone is responsible - can be examined further and more accurately.

## Sources
- https://groups.google.com/g/ocengine
- http://connectomeengine.com/
- http://connectomeengine.com/Home/Downloads
- https://www.researchgate.net/publication/263039837_Extending_the_C_Elegans_Connectome_to_Robotics_-_SOLID_2014_conference
- https://www.youtube.com/watch?v=2LWqN2NCnfU
- https://openworm.org/
