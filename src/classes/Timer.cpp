
#include "Timer.h"


Timer::Timer(int treshold_time, std::vector<Neuron*> t_neurons) 
: treshold_ms(treshold_time), target_neurons(t_neurons)
{      
}

void Timer::operator()(){
  int x = 0;//for dbg
  start_time = std::chrono::steady_clock::now();
  ellapsed_time = std::chrono::steady_clock::now() - start_time;
  while(RUN){
    ellapsed_time = std::chrono::steady_clock::now() - start_time;
    if(TO_MILLISEC(ellapsed_time).count() >= treshold_ms)
    {
      for(auto t : target_neurons) {
        t->get_input(0);
      }
      start_time = std::chrono::steady_clock::now();
      //std::cout << "Timer OK, ellapsed: " << x++ << "\n";
    }
  }
}
