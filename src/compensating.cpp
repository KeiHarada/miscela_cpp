// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// fillUpData()
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
#include "miscela.hpp"

void fillUpData(std::vector<Sensor> &s, int N){
   int size = int(s.size());
   for(int m=0;m<size;m++){
      std::map<int,int> interval = s[m].getINTERVAL();
      double begin = 0.0, end = 0.0;
      auto interval_itr = interval.begin();
      if(interval.begin()->first != 0){

      }
      while(interval_itr != interval.end()){

         begin = s[m].getDATA()[interval_itr->first];
         interval_itr++;
      }
   }

   return;
}