/*///////////////////////////////////////////////////////////////////
    Project: C. Elegans Connectome Emulation
    File: main.cpp
    Version: 1.2.1 /2022/
    Content: the main() function
    Purpose: 
      - Accessing the C. Elegans database
      - Building the connectome map 
      - Initializing neuron instances
      - Initializing connections between Neuron instances
      - Creating threads (multithreading)
      - Waiting for end signal
    Author: Norbert Keglevich

    More information can be found in 'README.md'
///////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cmath>
#include <chrono>
#include <unistd.h>
#include <thread>
#include <pthread.h>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>
#include <mysql_connection.h>
#include <driver.h>
#include <exception.h>
#include <resultset.h>
#include <statement.h>
#include <prepared_statement.h>

#include "globals.h" 
#include "classes.h"
#include "functions.h"



int main() {  
/***************************
  INTIALIZATION OF LOGGING
***************************/
    LOGFILE.open("log.txt", std::ios::out);
    if(!LOGFILE){
        std::cout << "Error creating log.txt\n";
    }
    else {
        LOGFILE << "log.txt created successfully\n";
    }    
  
  
  
  
/***************************************************************
  DECLARATION 
    - Neuron vectors
    - Connection Map: connectome
    - Connection Map selection: connection_ids
    - Other variables to assist in the initialization process
***************************************************************/
    std::vector<Neuron> cns_neurons;
    std::vector<Neuron*> motor_neurons;
    std::vector<Neuron*> sensors;  
    std::vector<Stimulator*> stimulators;
    std::vector<Connection*> connectome;
    std::vector<int> connection_ids;
  
    int conn_id {0};
    std::string cns_name;
    int cns_id {0};
  
    std::string connection_presyn;
    std::string connection_postsyn; 
    int connection_weight;
  
    std::string motor_name;
    int motor_id;
  
  
  
  
  
/*************************************************************************
  DATABASE MANAGEMENT 
    - Connecting to DB: c_elegans_db
    - Using tables: connectome_tbl, motor_neurons_tbl, cns_neurons_tbl
    - Data stored in tables are used to instantiate objects on the heap 
**************************************************************************/
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    std::unique_ptr<sql::PreparedStatement>  pstmt;
    std::unique_ptr<sql::ResultSet> res;
  
    driver = get_driver_instance();
    con = driver->connect("localhost","admin","password");
    con->setSchema("cpp_elegans_db"); 
  
  
  
  
  
/*******************************
  INITIALIZATION OF CONNECTOME   
*******************************/
    pstmt.reset(con->prepareStatement("SELECT * FROM connectome_tbl"));
    res.reset(pstmt->executeQuery());
  
    while (res->next()) {
        connection_presyn = res->getString("presynaptic");
        connection_postsyn = res->getString("postsynaptic");
        connection_weight = res->getInt("weight");

        connectome.push_back(new Connection(connection_presyn, 
                                            connection_postsyn, 
                                            connection_weight
                                           ));
    }
  
  
  
  
  
/**********************************
  INITIALIZATION OF MOTOR NEURONS   
**********************************/
    pstmt.reset(con->prepareStatement("SELECT * FROM motor_neurons_tbl"));
    res.reset(pstmt->executeQuery());
  
    while (res->next()) {
        motor_name = res->getString("name");
        motor_id = res->getInt("nid");
        motor_neurons.push_back(new MotorNeuron(motor_name, motor_id));
    }
  
  
  
  
    
/***********************************************************************
  INITIALIZATION OF CNS NEURONS
    - For every instance the connectome is traversed through 
      to create the connectome map selection to pass (connection_ids)  
***********************************************************************/
    pstmt.reset(con->prepareStatement("SELECT * FROM cns_neurons_tbl"));
    res.reset(pstmt->executeQuery());
  
    while (res->next()) {
        cns_name = res->getString("name");
        cns_id = res->getInt("nid");
        connection_ids.clear();
        conn_id = 0;

        for(auto c : connectome) {      
            if(!c->presynaptic.compare(cns_name)) {
                connection_ids.push_back(conn_id);        
            }
            conn_id++;
        }

        cns_neurons.push_back(Neuron(cns_name, cns_id, 
                                     connection_ids, 
                                     connectome
                                    ));
    }
  
  
  
  
  
    /******************************************
      FEEDING TARGETS OF CNS NEURONS
        - Pointers pointing to target neurons 
    ******************************************/
    for(int i{0}; i<cns_neurons.size(); ++i) { 
        for(int j{0}; j<connectome.size(); ++j) {      
            if(!connectome[j]->presynaptic.compare(cns_neurons[i].get_name())) {
                for(int k{0}; k<cns_neurons.size(); ++k){
                    if(!connectome[j]->postsynaptic.compare(cns_neurons[k].get_name())) {
                        cns_neurons[i].feed_target(&cns_neurons[k]);            
                    }          
                }

                for(int l{0}; l<motor_neurons.size(); ++l){
                    if(!connectome[j]->postsynaptic.compare(motor_neurons[l]->get_name())) {
                        cns_neurons[i].feed_target(motor_neurons[l]);            
                    }          
                }
            }      
        }
    }
  
  
  
  
  
/**********************************************************************
  INITIALIZATION OF SENSORS
    - For every instance the connectome is traversed through 
      to create the connectome map selection to pass (connection_ids)  
**********************************************************************/
    int id = 5001;
    for(auto sn : SENSOR_NAMES) {
        connection_ids.clear();
        conn_id = 0;

        for(auto c : connectome) {      
            if(!c->presynaptic.compare(sn)) {
                connection_ids.push_back(conn_id);        
            }
            conn_id++;
        }

        sensors.push_back(new Sensor(sn, id++, connection_ids, connectome));
    }
  
  
  
  
    /*******************************************
      FEEDING TARGETS OF SENSORS
        - Pointers pointing to target neurons 
    *******************************************/
    for(int i{0}; i<sensors.size(); ++i) { 
        for(int j{0}; j<connectome.size(); ++j) {      
            if(!connectome[j]->presynaptic.compare(sensors[i]->get_name())) {
                for(int k{0}; k<cns_neurons.size(); ++k){
                    if(!connectome[j]->postsynaptic.compare(cns_neurons[k].get_name())) {
                        sensors[i]->feed_target(&cns_neurons[k]);            
                    }
                }
            }
        }
    }
  
  
  
  
  
/********************************
  INITIALIZATION OF STIMULATORS
********************************/
    stimulators.push_back(new Stimulator ("noseStim", 6001, sensors[0]));
    stimulators.push_back(new Stimulator ("chemoStim", 6002, sensors[1]));
    stimulators.push_back(new Stimulator ("anteriorStim", 6003, sensors[2]));
    stimulators.push_back(new Stimulator ("posteriorStim", 6004, sensors[3]));
     
  
  
  
/***************************
  INITIALIZATION OF TIMERS
***************************/
    Timer motorTimer(MOTOR_TIME, motor_neurons);
    Timer sensorTimer(SENSOR_POLL, sensors);
  
  
  
  
  
/***************************************************
  SETTING UP THREADS
    - Separate scope for proper thread destruction
***************************************************/
    {
        std::vector<std::thread> cns_threads;

        for(int i{0}; i<cns_neurons.size(); ++i) {
            cns_threads.push_back(std::thread(cns_neurons[i]));
        }  

        std::thread muscle_timer_thread(motorTimer);
        std::thread sensor_timer_thread(sensorTimer);
        std::thread shell_thread(stimulator_shell, stimulators, std::ref(LOGFILE));

        while(RUN);

        for(int i{0}; i < cns_threads.size(); ++i) {
            cns_threads[i].join();
        }    

        muscle_timer_thread.join();
        sensor_timer_thread.join();
        shell_thread.join();  
    }
    
  
  
  
  
/**************
  TERMINATION
**************/
    logLine(LOGFILE, "Connectome terminated", true);
    LOGFILE.close();

    return 0;  
}   
  
  