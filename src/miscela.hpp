// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// header file of this project
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MISCELA_HPP
#define MISCELA_HPP
// ////////////////////////////////////////////////////////////////////////////////////////////////
// libraries //
#include <cstdlib>
#include <cfloat>
#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <time.h>
#include <stdexcept>
// ////////////////////////////////////////////////////////////////////////////////////////////////
// definition //
// value of no-measurement
const static double E = -100000000000.0;
// for DBSCAN
const static int MIN_PTS = 1;
const int NOISE = -2;
const int NOT_CLASSIFIED = -1;
// ////////////////////////////////////////////////////////////////////////////////////////////////
// class //
class Sensor{
private:
		int id;
		double lat;
		double lon;
		std::string attribute;
		std::vector<int> time;
		std::vector<double> data;
      std::vector<double> cmp;
      std::vector<double> mov;
		std::vector<double> reg;
		int ptsCnt;
		int cluster;
		std::vector<int> adj;
		bool adjmulti;
		std::vector<int> evolving_p;
		std::vector<int> evolving_n;
		std::map<int,int> interval;
		std::map<int,int> segment;
		std::map<int,double> slope;
public:
		// constructor
		Sensor();
		// accessor
		int getID();
		double getLAT();
		double getLON();
		std::string getATTRIBUTE();
		std::vector<int> getTIME();
		std::vector<double> getDATA();
		std::vector<double> getCOMPENSATE();
      std::vector<double> getMOVAVG();
		std::vector<double> getREG();
		std::vector<int> getEVOLVING_P();
		std::vector<int> getEVOLVING_N();
		std::map<int,int> getINTERVAL();
		std::map<int,int> getSEGMENT();
		std::map<int,double> getSLOPE();
		int getCLUSTER();
		int getPTS_CNT();
		std::vector<int> getADJ();
		bool checkADJMULTI();
		// setter
		void setLOCATION(std::string str);
		void setATTRIBUTE(std::string);
		void setDATA(std::string str);
		void setTestData();
		void setPTS(int x, std::string a);
		void setEVOLVING_P(int begin, int end);
		void setEVOLVING_N(int begin,int end);
		void setINTERVAL(int begin, int end);
		void setSEGMENT(int begin, int end);
		void setCOMPENSATE(int N);
		void setMOVAVG(int N);
		void setREG(int N);
		void setSLOPE(int key, double x);
		void setCLUSTER(int c);
};
// ////////////////////////////////////////////////////////////////////////////////////////////////
class Cluster{
private:
		std::vector<int> member;
		bool multiAttribute;
public:
		// constructor
		Cluster();
		// accessor
		std::vector<int> getMEMBER();
		bool checkMULTI();
		// setter
		void setCLUSTER(int i);
		void setMULTI();
};
// ////////////////////////////////////////////////////////////////////////////////////////////////
class Correlation{
private:
		bool flag;
		std::string attribute1;
		std::string attribute2;
	 	std::vector<std::string> attribute;
      std::vector<std::vector<int>> sensors;
		std::vector<int> sensors1;
		std::vector<int> sensors2;
		std::vector<int> timestamp;
		std::vector<int> tp;
		std::vector<int> tn;
		int association;
		int support;
public:
		// constructor
		Correlation(int PorN);
		Correlation(std::string a1, std::string a2, int PorN);
		// accessor
		bool getFLAG();
		std::string getATTRIBUTE1();
		std::string getATTRIBUTE2();
		std::vector<int> getSENSORS1();
		std::vector<int> getSENSORS2();
		std::vector<int> getTIMESTAMP();
		std::vector<int> getTP();
		std::vector<int> getTN();
		int getSUPPORT();
		int getASSOCIATION();
		// setter
		void setFLAG(bool f);
		void setATTRIBUTE1(std::string a1);
		void setATTRIBUTE2(std::string a2);
		void setSENSORS1(int number);
		void setSENSORS2(int number);
		void setTIMESTAMP(std::vector<int> p, std::vector<int> n);
};
// ////////////////////////////////////////////////////////////////////////////////////////////////
#endif //MISCELA_HPP