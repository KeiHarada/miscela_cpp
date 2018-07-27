// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// extractingDataInterval()
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
#include "miscela.hpp"

void movingAverage(std::vector<Sensor> &s, int N){

   int windowSize = 10;
   std::vector<double> x(windowSize+1, 0.0);
   double y;

   int sensorSize = int(s.size());
   for(int m=0;m<sensorSize;m++){
      std::vector<double> data = s[m].getCOMPENSATE();
      for(int n=0;n<N;n++){
         y = 0.0;
         x[windowSize] = data[n];
         for(int i=0;i<windowSize;i++){
            x[i] = x[i+1];
            y += x[i];
         }

      }

   }



   return;
}

