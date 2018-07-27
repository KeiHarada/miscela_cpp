#include "miscela.hpp"

/* prototype declaring of the functions */
void outputRawData(std::vector<Sensor> &, std::vector<int> &, std::vector<std::string> &, int);
void outputRegData(std::vector<Sensor> &, std::vector<int> &, std::vector<std::string> &, int);
void outputClusters(std::vector<Sensor> &, std::vector<Cluster> &);
std::string pinColor(int);

void outputData(std::vector<Sensor> &s, std::vector<Cluster> &c, std::vector<int> &M, std::vector<std::string> &AT, int N){

   outputRawData(s,M,AT,N);
   //outputRegData(s,M,AT,N);
   //outputClusters(s,c);

   return;
}

void outputRawData(std::vector<Sensor> &s, std::vector<int> &M, std::vector<std::string> &AT, int N){

   int m  = 0;
   for(int a=0;a<int(AT.size());a++){

      std::ofstream rawData("dataFile/rawData_"+AT[a]+".csv");
      if(!rawData){
         std::cout << "failed to open the file" << std::endl;
         exit(1);
      }

      std::string attribute = s[m].getATTRIBUTE();
      std::vector<Sensor> s_a;
      int start = m;
      while(attribute == AT[a]){
         s_a.push_back(s[m]);
         m++;
         attribute = s[m].getATTRIBUTE();
      }
      int end = m;


      for(int n = 0;n < 8000;n++){
         rawData << n;
         for(int i=start;i<end;i++){
            std::vector<double> data = s[i].getDATA();
            if(data[n] > E){
               rawData << "," << data[n];
            }else{
               rawData << ",";
            }
         }
         rawData << std::endl;
      }

   }

   return;
}

void outputRegData(std::vector<Sensor> &s, std::vector<int> &M, std::vector<std::string> &AT, int N){

   int m  = 0;
   for(int a=0;a<int(AT.size());a++){

      std::ofstream rawData("dataFile/regData_"+AT[a]+".csv");
      if(!rawData){
         std::cout << "failed to open the file" << std::endl;
         exit(1);
      }

      std::string attribute = s[m].getATTRIBUTE();
      std::vector<Sensor> s_a;
      int start = m;
      while(attribute == AT[a]){
         s_a.push_back(s[m]);
         m++;
         attribute = s[m].getATTRIBUTE();
      }
      int end = m;


      for(int n = 0;n < N;n++){
         rawData << n;
         for(int i=start;i<end;i++){
            std::vector<double> data = s[i].getREG();
            if(data[n] > E){
               rawData << "," << data[n];
            }else{
               rawData << ",";
            }
         }
         rawData << std::endl;
      }

   }

   return;
}

void outputClusters(std::vector<Sensor> &s, std::vector<Cluster> &clusters){

   std::ofstream cluster("dataFile/cluster2.csv");
   int colorSelector = 0;
   for(int i=0;i<int(clusters.size());i++){
      if(clusters[i].checkMULTI()){
         std::vector<int> c_i = clusters[i].getMEMBER();
         for(int j=0;j<int(c_i.size());j++){
            cluster << s[c_i[j]].getLAT() << ","
                    << s[c_i[j]].getLON() << ","
                    << s[c_i[j]].getATTRIBUTE() << "_"
                    << s[c_i[j]].getID() << ","
                    << pinColor(colorSelector) << std::endl;
         }
         colorSelector++;
      }
   }
   cluster << "cluster counter = " << colorSelector << std::endl;

   return;
}

std::string pinColor(int x){
   switch(x%27){
      case 0: return "pin-red";
      case 1: return "pin-blue";
      case 2: return "pin-green";
      case 3: return "pin-while";
      case 4: return "pin-yellow";
      case 5: return "pin-purple";
      case 6: return "circle-red";
      case 7: return "circle-blue";
      case 8: return "circle-green";
      case 9: return "circle-snow";
      case 10: return "circle-orange";
      case 11: return "circle-black";
      case 12: return "circle-magenta";
      case 13: return "circle-gold";
      case 14: return "circle-purple";
      case 15: return "circle-aquamarine";
      case 16: return "male1";
      case 17: return "male2";
      case 18: return "female1";
      case 19: return "female2";
      case 20: return "info1";
      case 21: return "stop1";
      case 22: return "fire1";
      case 23: return "thunder1";
      case 24: return "skull1";
      case 25: return "heart1";
      case 26: return "warning1";
      default: return "pin-red";
   }
}