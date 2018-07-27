#include "miscela.hpp"

/* main */
void pickingUpEvolving(std::vector<Sensor> &s, std::map<std::string,double> &THETA){
	int size = int(s.size());
	for(int m=0;m<size;m++){
		double theta = THETA[s[m].getATTRIBUTE()];
		std::map<int,int> segment = s[m].getSEGMENT();
		std::map<int,double> slope = s[m].getSLOPE();
		auto segment_itr = segment.begin();
		while(segment_itr != segment.end()){

			/* positive part */
			if(slope[segment_itr->first] >= theta){
				s[m].setEVOLVING_P(segment_itr->first,segment_itr->second);
			}

			/* negative part */
			if(slope[segment_itr->first] <= theta*(-1.0)){
				s[m].setEVOLVING_N(segment_itr->first,segment_itr->second);
			}

			segment_itr++;
		}
	}
}
