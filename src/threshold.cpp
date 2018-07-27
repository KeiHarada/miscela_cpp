#include "miscela.hpp"
void outputDistribution(std::vector<Sensor> &,std::vector<int> &);

std::map<std::string,double> thresholdEstimation(std::vector<Sensor> &s, std::vector<int> &size, double alpha){
	//outputDistribution(s,size);

	int x = 0;
	std::map<std::string,double> threshold;
	for(auto size_itr=size.begin();size_itr!=size.end();size_itr++){
		std::string attribute = s[x].getATTRIBUTE();
		/* check data range */
		double max = -1.0;
		for(int m=0;m<*size_itr;m++){
			std::map<int,double> slope = s[x+m].getSLOPE();
			for(auto slope_itr=slope.begin();slope_itr!=slope.end();slope_itr++){
				if(max < fabs(slope_itr->second)) max = fabs(slope_itr->second);
			}
		}

		// max count
		int count = 0;
		for(int m=0;m<*size_itr;m++){
			std::map<int,double> slope = s[x+m].getSLOPE();
			std::map<int,int> segment = s[x+m].getSEGMENT();
			for(auto slope_itr = slope.begin();slope_itr != slope.end();slope_itr++){
				if(fabs(slope_itr->second) > 0){
					for(int t = slope_itr->first;t < segment[slope_itr->first];t++){
						count++;
					}
				}
			}
		}
		std::cout << "test" << std::endl;
		// estimate
		int upper = count;
		int k = upper*alpha/100;
		double theta = (max)/1000.0;
		while(count > k){
			count = 0;
			for(int m=0;m<*size_itr;m++){
				std::map<int,double> slope = s[x+m].getSLOPE();
				std::map<int,int> segment = s[x+m].getSEGMENT();
				for(auto slope_itr=slope.begin();slope_itr!=slope.end();slope_itr++){
					if(fabs(slope_itr->second) > theta){
						for(int t=slope_itr->first;t<segment[slope_itr->first];t++){
							count++;
						}
					}
				}
			}
			theta += theta;
		}
/*
		// export file
		std::string path = "C:\\Users\\OnizukaLab\\Documents\\workspase\\MISCELA\\distribution\\";
		std::ofstream distribution(path+"distribution_"+s[x].getATTRIBUTE()+".csv");
		double theta = (max)/1000.0;
		for(double e=theta;e<=max;e+=theta){
			int count = 0;
			for(int m=0;m<*size_itr;m++){
				std::map<int,double> slope = s[x+m].getSLOPE();
				std::map<int,int> segment = s[x+m].getSEGMENT();
				for(auto slope_itr=slope.begin();slope_itr!=slope.end();slope_itr++){
					if(fabs(slope_itr->second) > e){
						for(int t=slope_itr->first;t<segment[slope_itr->first];t++){
							count++;
						}
					}
				}
			}
			distribution << e << "," << count << std::endl;
		}
*/
		threshold[attribute] = theta;
		x += *size_itr;
	}
	return threshold;
}

void outputDistribution(std::vector<Sensor> &s, std::vector<int> &size){
	int offset = 0;
	std::vector<std::vector<double>> counts;
	for(auto size_itr=size.begin();size_itr!=size.end();size_itr++){
		// max count
		int point = 0;
		for(int m=0;m<*size_itr;m++){
			std::map<int,double> slope = s[offset+m].getSLOPE();
			std::map<int,int> segment = s[offset+m].getSEGMENT();
			for(auto slope_itr = slope.begin();slope_itr != slope.end();slope_itr++){
				for(int t = slope_itr->first;t < segment[slope_itr->first];t++){
					point++;
				}
			}
		}
		std::vector<double> cs;
		for(int p=0;p<=100;p++){
			cs.push_back(point*p/(100*(*size_itr)));
		}
		counts.push_back(cs);
		offset += *size_itr;
	}
	// output
	std::ofstream distribution("distribution/distribution.csv");
	for(int p=0;p<=100;p++){
		distribution << p;
		for(int i=0;i<int(counts.size());i++){
			distribution << "," << counts[i][p];
		}
		distribution << std::endl;
	}
	return;
}