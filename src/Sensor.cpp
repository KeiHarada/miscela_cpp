// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// functions of the class "Sensor"
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
#include "miscela.hpp"
// ////////////////////////////////////////////////////////////////////////////////////////////////
// constructor //
Sensor::Sensor(){
	cluster = NOT_CLASSIFIED;
	ptsCnt = 0;
	adjmulti = false;
}
// ////////////////////////////////////////////////////////////////////////////////////////////////
// accessor //
int Sensor::getID(){ return id; }
double Sensor::getLAT(){ return lat; }
double Sensor::getLON(){ return lon; }
std::string Sensor::getATTRIBUTE(){ return attribute; }
std::vector<int> Sensor::getTIME(){ return time; }
std::vector<double> Sensor::getDATA(){ return data; }
std::vector<double> Sensor::getCOMPENSATE(){ return cmp; }
std::vector<double> Sensor::getMOVAVG(){ return mov; }
std::vector<double> Sensor::getREG(){ return reg; }
int Sensor::getCLUSTER(){ return cluster; }
int Sensor::getPTS_CNT(){ return ptsCnt; }
std::vector<int> Sensor::getADJ(){ return adj; }
bool Sensor::checkADJMULTI(){ return adjmulti; }
std::vector<int> Sensor::getEVOLVING_P(){ return evolving_p; }
std::vector<int> Sensor::getEVOLVING_N(){ return evolving_n; }
std::map<int,int> Sensor::getINTERVAL(){ return interval; }
std::map<int,int> Sensor::getSEGMENT(){ return segment; }
std::map<int,double> Sensor::getSLOPE(){ return slope;}
// ////////////////////////////////////////////////////////////////////////////////////////////////
// setter //
void Sensor::setATTRIBUTE(std::string a){ attribute = a; }
void Sensor::setINTERVAL(int begin, int end){ interval[begin] = end; }
void Sensor::setSLOPE(int key, double x){ slope[key] = x; }
void Sensor::setSEGMENT(int begin, int end){ segment[begin] = end; }
void Sensor::setCLUSTER(int c){ cluster = c; }
void Sensor::setLOCATION(std::string str){
   int column = 0;
   std::string token;
   std::istringstream stream(str);
   while(getline(stream,token,',')){
      switch(column){
         case 0: id = stoi(token); break;
         case 1: lat = stod(token); break;
         case 2: lon = stod(token); break;
         default: break;
      }
      column++;
   }
   return;
}
void Sensor::setDATA(std::string str){
   int column = 0;
	std::string token;
	std::istringstream stream(str);
	while(getline(stream,token,',')){
		switch(column){
			case 0: break;
			case 1: time.push_back(stoi(token)); break;
			case 2: data.push_back(stod(token)); break;
			default :break;
		}
		column++;
	}
}
void Sensor::setEVOLVING_P(int begin, int end){
	for(int i=begin;i<end;i++){
		evolving_p.push_back(i);
	}
	return;
}
void Sensor::setEVOLVING_N(int begin,int end){
	for(int i=begin;i<end;i++){
		evolving_n.push_back(i);
	}
	return;
}
void Sensor::setPTS(int x, std::string a){
	ptsCnt++;
	adj.push_back(x);
	if(attribute != a) adjmulti = true;
	return;
}
void Sensor::setREG(int N){
	auto segment_itr = segment.begin();
	int n = 0;
	while(n < N){
		while(n<segment_itr->first){
			reg.push_back(E);
			n++;
		}
		if(n == N) break;
		if(segment_itr == segment.end()){
			while(n < N){
				reg.push_back(E);
				n++;
			}
		}
		if(n == N) break;
		double a = (data[segment_itr->second]-data[segment_itr->first])/(segment_itr->second-segment_itr->first);
		double b = data[segment_itr->first]-a*segment_itr->first;
		while(n<segment_itr->second){
			reg.push_back(a*n+b);
			n++;
		}
		if(n == N) break;
		segment_itr++;
		if(n != segment_itr->first){
			reg.push_back(data[n]);
			n++;
		}
	}
	return;
}
void Sensor::setCOMPENSATE(int N){
   auto interval_itr = interval.begin();
   auto interval_final = interval.end();
   interval_final--;
   int n = 0;

   while(n<interval.begin()->first){
      cmp.push_back(data[interval_itr->first]);
      n++;
   }

   while(n < N){

      while(n<interval_itr->second){
         cmp.push_back(data[n]);
         n++;
      }

      if(interval_itr == interval_final){
         while(n < N){
            cmp.push_back(data[interval_itr->second]);
            n++;
         }
      }
      if(n == N) break;

      int x1 = interval_itr->second;
      double y1 = data[interval_itr->second];
      interval_itr++;
      int x2 =  interval_itr->first;
      double y2 = data[interval_itr->first];

      double a = (y2-y1)/(x2-x1);
      double b = y1-a*x1;
      while(n<interval_itr->first){
         cmp.push_back(a*n+b);
         n++;
      }
   }
   return;
}

void Sensor::setMOVAVG(int N){

   int windowSize = 12;
   std::vector<double> x(windowSize+1, 0.0);
   double y;

   for(int n=0;n<windowSize;n++){
      mov.push_back(cmp[n]);
      x[n] = cmp[n];
   }

   for(int n=windowSize;n<N;n++){
      y = 0.0;
      x[windowSize] = cmp[n];
      for(int i=0;i<windowSize;i++){
         x[i] = x[i+1];
         y += x[i];
      }
      mov.push_back(y/windowSize);
   }

   return;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
/*
void Sensor::setTestData(){
	double lower,upper;
	if(attribute == "temperature"){
		lower = 18.0;
		upper = 28.0;
	}
	if(attribute == "noise"){
		lower = 40.0;
		upper = 60.0;
	}
	if(attribute == "light"){
		lower = 100.0;
		upper = 100000.0;
	}
	if(attribute == "irrigation"){
		lower = 50.0;
		upper = 80.0;
	}
	if(attribute == "traffic"){
		lower = 10.0;
		upper = 90.0;
	}

	int length = 10;
	double rate = (upper-lower)/length;
	double avg = (lower+upper)/2;

	int t = 3010000;

	for(int i=0;i<N;i++){
		data.push_back(-1.0);
		time.push_back(t);
		t = increment(t);
	}

	std::vector<double> change;
	for(int t=0;t<length;t++){
		change.push_back(lower+t*rate);
	}
	for(int t=length;t>0;t--){
		change.push_back(lower+t*rate);
	}

	std::mt19937_64 mt(id);
	std::uniform_int_distribution<> rand(0,N-length*2);

	int changeCnt = N/100;
	for(int i=0;i<changeCnt;i++){
		bool flag;
		int start;
		do{
			flag = true;
			start = rand(mt);
			for(int i=start;i<start+2*length;i++){
				if(data[i] > 0) flag = false;
			}
		}while(!flag);
		auto itr = change.begin();
		for(int i=start;i<start+2*length;i++){
			data[i] = *itr;
			itr++;
		}
	}

	for(int n=0;n<N;n++){
		if(fabs(data[n]-(-1.0)) < 0.01) data[n] = avg;
	}
}
 */