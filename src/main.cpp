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
#include "functions/declarations.h"
#include "classes/Connection.h"
#include "classes/Neuron.h"
#include "classes/MotorNeuron.h"
#include "classes/Stimulator.h"
#include "classes/Timer.h"




int main() {  
  //std::fstream logFile;
  LOGFILE.open("log.txt", std::ios::out);
  if(!LOGFILE){
    std::cout << "Error creating log.txt\n";
  }
  else {
    LOGFILE << "log.txt created successfully\n";
  }    

  std::vector<Neuron> cns_neurons;
  std::vector<Neuron*> motor_neurons;
  std::vector<Neuron*> sensors;
  std::vector<std::string> sensor_names{
    "noseSens",
    "chemoSens",
    "anteriorSens",
    "posteriorSens"
  };
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
    
  //DATABASE MANAGEMENT
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  std::unique_ptr<sql::PreparedStatement>  pstmt;
  std::unique_ptr<sql::ResultSet> res;

  driver = get_driver_instance();
  con = driver->connect("localhost","admin","password");
  con->setSchema("cpp_elegans_db"); 


  //CONNECTOME INIT
  pstmt.reset(con->prepareStatement("SELECT * FROM connectome_tbl"));
  res.reset(pstmt->executeQuery());

  while (res->next()) {
    connection_presyn = res->getString("presynaptic");
    connection_postsyn = res->getString("postsynaptic");
    connection_weight = res->getInt("weight");
    connectome.push_back(new Connection(connection_presyn, connection_postsyn, connection_weight));
  }


  //MOTOR NEURONS INIT
  pstmt.reset(con->prepareStatement("SELECT * FROM motor_neurons_tbl"));
  res.reset(pstmt->executeQuery());
  //FEED TARGETS FOR MOTOR NEURONS

  while (res->next()) {
    motor_name = res->getString("name");
    motor_id = res->getInt("nid");
    motor_neurons.push_back(new MotorNeuron(motor_name, motor_id));
  }

  
  //CNS NEURONS INIT
  pstmt.reset(con->prepareStatement("SELECT * FROM cns_neurons_tbl"));
  res.reset(pstmt->executeQuery());

  while (res->next()) {
    cns_name = res->getString("name");
    cns_id = res->getInt("nid");
    connection_ids.clear();
    conn_id = 0;
    for(auto c : connectome) {      
      if(!c->presynaptic.compare(cns_name)) {
        std::cout << cns_name << " targets: " 
        << c->postsynaptic << " | weight: " <<c->weight << "\n";
        connection_ids.push_back(conn_id);        
      }
      conn_id++;
    }
    cns_neurons.push_back(Neuron(cns_name, cns_id, connection_ids, connectome));
  }
  //FEED TARGETS FOR CNS NEURONS
  for(int i{0}; i<cns_neurons.size(); i++) { 
    for(int j{0}; j<connectome.size(); j++) {      
      if(!connectome[j]->presynaptic.compare(cns_neurons[i].get_name())) {
        for(int k{0}; k<cns_neurons.size(); k++){
          if(!connectome[j]->postsynaptic.compare(cns_neurons[k].get_name())) {
            cns_neurons[i].feed_target(&cns_neurons[k]);            
          }          
        }
        for(int l{0}; l<motor_neurons.size(); l++){
          if(!connectome[j]->postsynaptic.compare(motor_neurons[l]->get_name())) {
            cns_neurons[i].feed_target(motor_neurons[l]);            
          }          
        }
      }      
    }
  }


  //SENSORS INIT
  int id = 5001;
  for(auto sn : sensor_names) {
    connection_ids.clear();
    conn_id = 0;
    for(auto c : connectome) {      
      if(!c->presynaptic.compare(sn)) {
        std::cout << sn << " targets: " 
        << c->postsynaptic << " | weight: " << c->weight << "\n";
        connection_ids.push_back(conn_id);        
      }
      conn_id++;
    }
    sensors.push_back(new Sensor(sn, id++, connection_ids, connectome));
  }
  //FEED TARGETS FOR SENSORS 
  for(int i{0}; i<sensors.size(); i++) { 
    for(int j{0}; j<connectome.size(); j++) {      
      if(!connectome[j]->presynaptic.compare(sensors[i]->get_name())) {
        for(int k{0}; k<cns_neurons.size(); k++){
          if(!connectome[j]->postsynaptic.compare(cns_neurons[k].get_name())) {
            sensors[i]->feed_target(&cns_neurons[k]);            
          }
        }
      }
    }
  }
  
  Stimulator noseStim("noseStim", 6001, sensors[0]);
  Stimulator chemoStim("chemoStim", 6002, sensors[1]);
  Stimulator anteriorStim("anteriorStim", 6003, sensors[2]);
  Stimulator posteriorStim("posteriorStim", 6004, sensors[3]);

  std::vector<Stimulator*> stimulators {
    &noseStim,
    &chemoStim,
    &anteriorStim,
    &posteriorStim
  };

  Timer motorTimer(MOTOR_TIME, motor_neurons);
  Timer sensorTimer(SENSOR_POLL, sensors);

  //THREADS
  {//scope for threads (for proper destruction)
    std::vector<std::thread> cns_threads;
    for(int i{0}; i<cns_neurons.size(); i++) {
      cns_threads.push_back(std::thread(cns_neurons[i]));
    }  
  
    std::thread muscle_timer_thread(motorTimer);
    std::thread sensor_timer_thread(sensorTimer);
    std::thread shell_thread(stimulator_shell, stimulators, std::ref(LOGFILE));
  
    while(RUN);

    for(int i{0}; i < cns_threads.size(); i++) {
      cns_threads[i].join();
    }    
    muscle_timer_thread.join();
    sensor_timer_thread.join();
    shell_thread.join();
  
  }//scope for threads 
  

  logLine(LOGFILE, "Connectome terminated", true);
  LOGFILE.close();
  
  return 0;  
} 










