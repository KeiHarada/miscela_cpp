#include "miscela.hpp"

const static double MAX_ERROR_t = 2.0; // have to tune.
const static double MAX_ERROR_n = 2.0; // have to tune.
const static double MAX_ERROR_l = 2000.0; // have to tune.
const static double MAX_ERROR_i = 5.0; // have to tune.
const static double MAX_ERROR_tr = 10.0; // have to tune.
const static double MAX_ERROR_pm25 = 30.0; // have to tune.
const static double MAX_ERROR_pm10 = 20.0; // have to tune.
const static double MAX_ERROR_no2 = 5.0; // have to tune.

/* prototype declaring of the functions */
void segmenting(Sensor &,int,int,double);

/* main */
void segmentingDataInterval(std::vector<Sensor> &s, int N){
   int size = int(s.size());
   for(int m=0;m<size;m++){
      double error;
      if(s[m].getATTRIBUTE() == "temperature") error = MAX_ERROR_t;
      if(s[m].getATTRIBUTE() == "noise") error = MAX_ERROR_n;
      if(s[m].getATTRIBUTE() == "light") error = MAX_ERROR_l;
      if(s[m].getATTRIBUTE() == "irrigation") error = MAX_ERROR_i;
      if(s[m].getATTRIBUTE() == "traffic") error = MAX_ERROR_tr;
      if(s[m].getATTRIBUTE() == "pmp25") error = MAX_ERROR_pm25;
      if(s[m].getATTRIBUTE() == "pm10") error = MAX_ERROR_pm10;
      if(s[m].getATTRIBUTE() == "no2") error = MAX_ERROR_no2;
      std::map<int,int> interval = s[m].getINTERVAL();
      auto interval_itr = interval.begin();

      while(interval_itr != interval.end()){
         /* omit the isolated point */
         if((interval_itr->second-interval_itr->first) > 0){
            segmenting(s[m],interval_itr->first,interval_itr->second,error);
         }
         interval_itr++;
      }
      /* calculate the regression line */
      s[m].setREG(N);
   }
}

void segmenting(Sensor &s, int begin, int end, double error){
   std::vector<double> data = s.getDATA();

   /* fix the segment on this interval if it has only 2 timestamps */
   if(end-begin == 1){
      s.setSEGMENT(begin,end);
      s.setSLOPE(begin,(data[end]-data[begin]));
   }else{
      double prev,current;
      std::vector<double> cost;
      std::map<int,int> seg;

      /* calculate initial costs */
      for(int i=begin;i<end;i++){
         current = data[i+1]-data[i];
         seg[i] = i+1;
         if(i > begin) cost.push_back(fabs(prev-current));
         prev = current;
      }

      /* merge neighbors */
      auto cost_itr = min_element(cost.begin(),cost.end());
      int  cost_mini = distance(cost.begin(),cost_itr);
      auto seg_itr = seg.find(begin+cost_mini);
      int seg_start = seg_itr->first;
      while(cost[cost_mini] < error && !cost.empty()){

         /* update seg[begin+mini] */
         int next = seg[seg[seg_start]];
         seg.erase(seg[seg_start]);
         seg[seg_start] = next;

         /* erase cost[mini] */
         cost_itr = cost.erase(cost_itr);

         if(!cost.empty()){
            /* update cost[mini] */
            if(cost_itr!=cost.end()){
               prev = (data[seg[seg_start]]-data[seg_start])/(seg[seg_start]-seg_start);
               current = (data[seg[seg[seg_start]]]-data[seg[seg_start]])/(seg[seg[seg_start]]-seg[seg_start]);
               cost[cost_mini] = fabs(prev-current);
            }

            /* update cost[mini-1] */
            if(cost_itr!=cost.begin()){
               seg_itr--;
               prev = (data[seg_start]-data[seg_itr->first])/(seg_start-seg_itr->first);
               current = (data[seg[seg_start]]-data[seg_start])/(seg[seg_start]-seg_start);
               cost[cost_mini-1] = fabs(prev-current);
            }

            /* re-calculate minimum cost */
            cost_itr = min_element(cost.begin(),cost.end());
            cost_mini = distance(cost.begin(),cost_itr);
            seg_itr = seg.begin();
            advance(seg_itr,cost_mini);
            seg_start = seg_itr->first;
         }
      }

      /* set segments on this interval */
      for(auto itr = seg.begin();itr != seg.end();itr++){
         double slope = (data[itr->second]-data[itr->first])/(itr->second-itr->first);
         if(itr->second != 0){
            s.setSEGMENT(itr->first, itr->second);
            s.setSLOPE(itr->first, slope);
         }
      }
   }
}