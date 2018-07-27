#include "miscela.hpp"

/* constructor */
Correlation::Correlation(int PorN){
	flag = false;
	association = PorN;
	support = 0;
}

Correlation::Correlation(std::string a1, std::string a2,int PorN){
	flag = false;
	association = PorN;
	support = 0;
	attribute1 = a1;
	attribute2 = a2;
}

/* accessor */
bool Correlation::getFLAG(){ return flag; }
std::string Correlation::getATTRIBUTE1(){ return attribute1; }
std::string Correlation::getATTRIBUTE2(){ return attribute2; }
std::vector<int> Correlation::getSENSORS1(){ return sensors1; }
std::vector<int> Correlation::getSENSORS2(){ return sensors2; }
std::vector<int> Correlation::getTIMESTAMP(){ return timestamp; }
std::vector<int> Correlation::getTP(){return tp; }
std::vector<int> Correlation::getTN(){return tn; }
int Correlation::getSUPPORT(){ return support; }
int Correlation::getASSOCIATION(){ return association; }

/* setter */
void Correlation::setFLAG(bool f){
	flag = f;
}

void Correlation::setATTRIBUTE1(std::string a1){
	attribute1 = a1;
}

void Correlation::setATTRIBUTE2(std::string a2){
	attribute2 = a2;
}

void Correlation::setSENSORS1(int number){
	sensors1.push_back(number);
	sort(sensors1.begin(),sensors1.end());
}

void Correlation::setSENSORS2(int number){
	sensors2.push_back(number);
	sort(sensors2.begin(),sensors2.end());
}

void Correlation::setTIMESTAMP(std::vector<int> p,std::vector<int> n){
	tp.clear();
	tp = p;
	tp.shrink_to_fit();
	tn.clear();
	tn = n;
	tn.shrink_to_fit();
	support = int(tp.size())+int(tn.size());
	timestamp.clear();
	for(auto itr=tp.begin();itr!=tp.end();itr++){
		timestamp.push_back(*itr);
	}
	for(auto itr=tn.begin();itr!=tn.end();itr++){
		timestamp.push_back(*itr);
	}
	sort(timestamp.begin(),timestamp.end());
}
