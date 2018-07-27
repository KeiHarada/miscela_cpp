// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// main program of MISCELA
//
// arguments:
// 1. algorithm selector "miscela" or "base"
// 2. data set selector "place:sampling rate" i.e. "santander:par15min"
// 3. minimum support "psi"
// 4. evolving rate "alpha"
// 5. distance threshold "h"
// 6. the number of attributes "K"
// 7. attribute 1
// |
// 6+K. attribute K
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
// feader file
#include "miscela.hpp"
// ////////////////////////////////////////////////////////////////////////////////////////////////
// functions
std::vector<std::string> split(const std::string &, char);
int inputData(std::vector<Sensor> &,std::string,std::vector<std::string> &);
double clusteringSensors(std::vector<Cluster> &,std::vector<Sensor> &);
double clusteringSensors(std::vector<Cluster> &,std::vector<Sensor> &,double);
void extractingDataInterval(std::vector<Sensor> &,int);
void segmentingDataInterval(std::vector<Sensor> &,int);
std::map<std::string,double> thresholdEstimation(std::vector<Sensor> &,std::vector<int> &,double);
void pickingUpEvolving(std::vector<Sensor> &,std::map<std::string,double> &);
std::vector<int> findAssociation(std::vector<Cluster> &,std::vector<Sensor> &,int,int,double);
std::vector<int> findAssociation_base(std::vector<Cluster> &,std::vector<Sensor> &,std::string,std::string,int,int,double);
void outputData(std::vector<Sensor> &, std::vector<Cluster> &, std::vector<int> &, std::vector<std::string> &, int);
// ////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc,char*argv[]){

   // given parameters
   std::string selector = argv[1]; // algorithm selector
   std::vector<std::string> dataSet = split(argv[2],':');
   const static int psi = std::stoi(argv[3]); // minimum support
   const static double alpha = std::stod(argv[4]); // evolving rate
   double h = std::stod(argv[5]); // distance threshold
   const static int K = std::stoi(argv[6]); // the number of attributes
   std::vector<std::string> A; // attributes
   for(int i = 1; i <= K; i++){
      A.push_back(argv[6 + i]);
   }

   // output file
   std::ofstream Computation(
           "experiment/" + dataSet[0] + "_" + selector + "_attribute" + std::to_string(K) + "_Computation.csv");
   std::ofstream Time("experiment/" + dataSet[0] + "_" + selector + "_attribute" + std::to_string(K) + "_Time.csv");

   // for results
   std::vector<int> correlationCounter; // [0]:the number of computations, [1]:the number of correlation
   int clusterCounter = 0; // the number of clusters
   double H; // decided distance

   // average time keeper
   double avgCorrelation = 0.0;
   double avgCluster = 0.0;
   double avgPre = 0.0;

   // repeat the experiment
   int loop = 1;
   for(int i = 0; i < loop; i++){

      // time keeper
      clock_t start,end;
      double time;

      // input data
      std::vector<Sensor> s;
      std::vector<int> M;
      for(int a = 0; a < K; a++){
         std::vector<Sensor> s_a;
         M.push_back(inputData(s_a,A[a],dataSet));
         s.insert(s.end(),s_a.begin(),s_a.end());
      }

      std::ofstream sl("./china_sensors.txt");
      for(int m = 0; m < s.size(); m++){
         std::string att = "";
//         if(s[m].getATTRIBUTE() == "temperature") att = "pin-red";
//         if(s[m].getATTRIBUTE() == "light") att = "pin-yellow";
//         if(s[m].getATTRIBUTE() == "noise") att = "pin-white";
//         if(s[m].getATTRIBUTE() == "traffic") att = "pin-green";
//         if(s[m].getATTRIBUTE() == "irrigation") att = "pin-blue";

         if(s[m].getATTRIBUTE() == "pm25") att = "pin-red";
         if(s[m].getATTRIBUTE() == "pm10") att = "pin-yellow";
         if(s[m].getATTRIBUTE() == "no2") att = "pin-white";


         sl << s[m].getLAT() << "," << s[m].getLON() << "," << s[m].getID() << "," << att << std::endl;
      }

//      // length of timestamps
//      int N = (s[0].getDATA().size());
//
//      // pre-processing
//      start = clock();
//      extractingDataInterval(s,N);
//      segmentingDataInterval(s,N);
//      std::cout << "fisish" << std::endl;
//      std::map<std::string,double> theta = thresholdEstimation(s,M,alpha);
//      //pickingUpEvolving(s,theta);
//      end = clock();
//      time = double(end - start) / CLOCKS_PER_SEC;
//      avgPre += time;
//   }
//
//
//      // (1) base algorithm
//      if(selector == "base"){
//
//         int total_cmp = 0;
//         int total_cor = 0;
//
//         for(int i = 0; i < M_a; i++){
//            for(int j = i + 1; j < M_a; j++){
//               std::vector<Sensor> s_ij;
//
//               // set attribute 1
//               int begin_i = 0;
//               for(int x = 0; x < i; x++){
//                  begin_i += M[x];
//               }
//               for(int m = begin_i; m < begin_i + M[i]; m++){
//                  s_ij.push_back(s[m]);
//               }
//
//               // set attribute 2
//               int begin_j = 0;
//               for(int x = 0; x < j; x++){
//                  begin_j += M[x];
//               }
//               for(int m = begin_j; m < begin_j + M[j]; m++){
//                  s_ij.push_back(s[m]);
//               }
//
//               std::cout << std::endl
//                         << "attribute1: " << ATT[i]
//                         << " , "
//                         << "attribute2: " << ATT[j]
//                         << std::endl;
//
//               // clustering
//               start = clock();
//               std::vector<Cluster> clusters;
//               H = clusteringSensors(clusters,s_ij,h); // using DBSCAN
//               end = clock();
//               time = double(end - start) / CLOCKS_PER_SEC;
//               avgCluster += time;
//               clusterCounter = int(clusters.size());
//               std::cout << "clustering : " << time << "[s]" << std::endl;
//
//               // CAP search
//               start = clock();
//               correlationCounter = findAssociation_base(clusters,s_ij,ATT[i],ATT[j],N,MIN_SUP,H);
//               end = clock();
//               time = double(end - start) / CLOCKS_PER_SEC;
//               avgCorrelation += time;
//               total_cmp += correlationCounter[0];
//               total_cor += correlationCounter[1];
//               std::cout << "CAP search : " << time << "[s]" << std::endl;
//            }
//         }
//         correlationCounter[0] = total_cmp;
//         correlationCounter[1] = total_cor;
//      }
//
//      // (2) miscela
//      if(selector == "miscela"){
//
//         // clustering
//         start = clock();
//         std::vector<Cluster> clusters;
//         //H = clusteringSensors(clusters,s); // using OPTICS
//         H = clusteringSensors(clusters,s,h); // using DBSCAN
//         end = clock();
//         std::cout << "H = " << H << std::endl;
//         time = double(end - start) / CLOCKS_PER_SEC;
//         avgCluster += time;
//         clusterCounter = int(clusters.size());
//         std::cout << "clustering : " << time << "[s]" << std::endl;
//
//         // CAP search
//         start = clock();
//         correlationCounter = findAssociation(clusters,s,N,MIN_SUP,H);
//         end = clock();
//         time = double(end - start) / CLOCKS_PER_SEC;
//         avgCorrelation += time;
//         std::cout << "correlation : " << time << "[s]" << std::endl;
//         clusters.shrink_to_fit();
//         //outputData(s,clusters,M,AT,N); // uncomment this if you want to display detail of results
//      }
//   }
//
//   // display results
//   avgCluster = avgCluster / loop;
//   avgCorrelation = avgCorrelation / loop;
//   avgPre = avgPre / loop;
//   std::cout << std::endl;
//   std::cout << "pre-processing : " << avgPre << "[s]" << std::endl;
//   std::cout << "clustering : " << avgCluster << "[s]" << std::endl;
//   std::cout << "CAP search : " << avgCorrelation << "[s]" << std::endl;
//   std::cout << "# of computations : " << correlationCounter[0] << std::endl;
//   std::cout << "# of correlations : " << correlationCounter[1] << std::endl;
//   std::cout << "estimated distance threshold : " << H << "[km]" << std::endl;
//   std::cout << "# of clusters : " << clusterCounter << std::endl;
//
//   // export to files
//   Computation << M_a << "," << correlationCounter[0] << std::endl;
//   Time << M_a << "," << avgCorrelation << std::endl;

      return 0;
   }

}

std::vector<std::string> split(const std::string &str, char sep){
   std::vector<std::string> v;
   std::stringstream ss(str);
   std::string buffer;
   while( std::getline(ss, buffer, sep) ) {
      v.push_back(buffer);
   }

   return v;
}
