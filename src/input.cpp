// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// inputData()
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
#include "miscela.hpp"
std::vector<std::string> split(const std::string &, char);
// ////////////////////////////////////////////////////////////////////////////////////////////////
int inputData(std::vector<Sensor> &s, std::string attribute, std::vector<std::string> &dataSet){
	std::string inputSelector = "./input/"+dataSet[0]+"/"+dataSet[1]+"/";

	// file open
	std::ifstream  ifsLocations(inputSelector+"location_"+attribute+".txt");
	if(ifsLocations.fail()){
		std::cerr << "location_"+attribute+".txt don't exist.\n";
		exit(0);
	}
	std::ifstream  ifsMeasurements(inputSelector+"measurement_"+attribute+".txt");
	if(ifsMeasurements.fail()){
		std::cerr << "measurement_"+attribute+".txt don't exist.\n";
		exit(0);
	}

	// set sensor info and input their reads
   std::string strLocations;
   std::queue<std::string> q;
   while(getline(ifsLocations,strLocations)){
      Sensor s_i;
      s_i.setLOCATION(strLocations);
      s_i.setATTRIBUTE(attribute);
      int id = s_i.getID();
      int _id = s_i.getID();

      std::string strMeasurements;
      std::vector<std::string> buffer;
      while(!q.empty()){
         s_i.setDATA(q.front());
         q.pop();
      }

      while(_id == id ){
         if(getline(ifsMeasurements,strMeasurements)){
            _id = std::stoi(split(strMeasurements,',')[0]);
            if(_id == id){
               s_i.setDATA(strMeasurements);
            }else{
               q.push(strMeasurements);
            }
         }else{
            id = -1;
         }
      }
      s.push_back(s_i);
   }

	// file close
	ifsLocations.close();
	ifsMeasurements.close();

   return int(s.size());
}
// ////////////////////////////////////////////////////////////////////////////////////////////////