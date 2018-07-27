#include "miscela.hpp"

/* global */
static std::map<int,int> v;
static int asCounter = 0;
static int cmpCount = 0;

/* struct */
struct sensorSet{
		std::string attribute1;
		std::string attribute2;
		std::vector<int> member;
};

/* prototype declaring of functions */
void Search(Cluster &,std::vector<Sensor> &,sensorSet,Correlation,Correlation,int,int,double);
bool checkMultiAttribute(std::vector<Sensor> &,std::vector<int> &);
bool checkParent(std::vector<Sensor> &,sensorSet &,sensorSet,double);
bool checkConnection(std::vector<Sensor> &,std::vector<int> &,double);
void visit(std::vector<Sensor> &,std::vector<int> & ,int,double);
void setAssociation(Sensor &,int,Correlation &, Correlation &,int);
void copyAssociation(Correlation ,Correlation ,Correlation &,Correlation &);
void outputAssociation(std::vector<Sensor> &,Correlation &,int);
std::vector<int> getNeighbors(Cluster &,std::vector<Sensor> &,sensorSet &);
void evaluation(std::vector<int> &,int,int);
double dist1(double,double,double,double);
double dist2(double,double,double,double);

/* main */
std::vector<int> findAssociation(std::vector<Cluster> &clusters, std::vector<Sensor> &s, int N, int MINI_SUP, double H){
	/* initial setting */
	Correlation ap(1);
	Correlation an(-1);
	sensorSet x;
	asCounter = 0;
	cmpCount = 0;

	for(int i=0;i<int(clusters.size());i++){
		if(clusters[i].checkMULTI()){
			Search(clusters[i],s,x,ap,an,N,MINI_SUP,H);
		}
	}
	//std::cout << asCounter << " sensor sets were found" << std::endl;
	//std::cout << "Comparision Count : " << cmpCount << std::endl;

	std::vector<int> result = {cmpCount,asCounter};
	return  result;
}

void Search(Cluster &c, std::vector<Sensor> &s, sensorSet x, Correlation ap_x, Correlation an_x, int N, int MINI_SUP,double H){
	/* output association */
	if(int(x.member.size()) >= 2 && ap_x.getFLAG()){
		asCounter++;
		//std::vector<int> timestamp = ap_x.getTIMESTAMP();
		//evaluation(timestamp,N,asCounter);
		/*if(int(ap_x.getSENSORS1().size()) >= 2 && int(ap_x.getSENSORS2().size()) >= 2)*/
		if(ap_x.getATTRIBUTE1()=="irrigation" || ap_x.getATTRIBUTE2()=="irrigation") outputAssociation(s,ap_x,N);
	}
	if(int(x.member.size()) >= 2 && an_x.getFLAG()){
		asCounter++;
		//std::vector<int> timestamp = an_x.getTIMESTAMP();
		//evaluation(timestamp,N,asCounter);
      /*if(int(an_x.getSENSORS1().size()) >= 2 && int(an_x.getSENSORS2().size()) >= 2)*/
		if(an_x.getATTRIBUTE1()=="irrigation" || an_x.getATTRIBUTE2()=="irrigation") outputAssociation(s,an_x,N);
	}

	/* find neighbor nodes */
	std::vector<int> ns = getNeighbors(c,s,x);

	/* search*/
	for(auto ns_itr=ns.begin();ns_itr!=ns.end();ns_itr++){
		sensorSet y = x;
		y.member.push_back(*ns_itr);
		switch(int(y.member.size())){
			case 1:
				y.attribute1 = s[y.member[0]].getATTRIBUTE();
				break;
			case 2:
				y.attribute2 = s[y.member[1]].getATTRIBUTE();
				break;
			default: break;
		}
		sort(y.member.begin(),y.member.end());

		/* keep searching if x is a parent of y*/
		if(checkParent(s,x,y,H)){
			Correlation ap_y(1);
			Correlation an_y(-1);

			/* if y has only a node */
			switch(int(y.member.size())){
				case 1:
					ap_y.setTIMESTAMP(s[y.member[0]].getEVOLVING_P(),s[y.member[0]].getEVOLVING_N());
					an_y.setTIMESTAMP(s[y.member[0]].getEVOLVING_P(),s[y.member[0]].getEVOLVING_N());
					ap_y.setSENSORS1(y.member[0]);
					an_y.setSENSORS1(y.member[0]);
					ap_y.setATTRIBUTE1(y.attribute1);
					an_y.setATTRIBUTE1(y.attribute1);
					if(ap_y.getSUPPORT() >= MINI_SUP) ap_y.setFLAG(true);
					if(an_y.getSUPPORT() >= MINI_SUP) an_y.setFLAG(true);
					break;
				case 2:
					copyAssociation(ap_x,an_x,ap_y,an_y);
					ap_y.setATTRIBUTE2(y.attribute2);
					an_y.setATTRIBUTE2(y.attribute2);
					setAssociation(s[*ns_itr],*ns_itr,ap_y,an_y,MINI_SUP);
					break;
				default:
					copyAssociation(ap_x,an_x,ap_y,an_y);
					setAssociation(s[*ns_itr],*ns_itr,ap_y,an_y,MINI_SUP);
					break;
			}

			/* recursive */
			if(ap_y.getFLAG() || an_y.getFLAG()) Search(c,s,y,ap_y,an_y,N,MINI_SUP,H);
		}
	}
}

bool checkParent(std::vector<Sensor> &s, sensorSet &parent, sensorSet child,double H){
	switch(int(parent.member.size())){
		case 0: return true;
		case 1:
			child.member.erase(child.member.begin());
			if(child.member == parent.member){
				return true;
			}else{
				return false;
			}
		default:
			/* roll up operation */
			for(auto child_itr=child.member.begin();child_itr!=child.member.end();child_itr++){
				int keep = *child_itr;
				child.member.erase(child_itr);
				/* if X = Y - {s} is still connected and has multi-attribute sensors */
				if(checkConnection(s,child.member,H) && checkMultiAttribute(s,child.member)){
					if(child.member == parent.member){
						return true;
					}else{
						return false;
					}
				}else{
					child.member.insert(child_itr,keep);
				}
			}
	}

	return true;
}

bool checkConnection(std::vector<Sensor> &s, std::vector<int> &graph, double H){
	if(int(graph.size()) == 1) return true;

	v.clear();
	/* start DFS */
	visit(s,graph,graph[0],H);

	/* check connection */
	for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
		if(v.find(*graph_itr) == v.end()){
			return false;
		}
	}
	return true;
}

void visit(std::vector<Sensor> &s, std::vector<int> &graph, int start, double H){

	/* DFS algorithm */
	v[start] = 1;
	for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
		if(dist2(s[start].getLAT(),s[start].getLON(),s[*graph_itr].getLAT(),s[*graph_itr].getLON()) <= H &&
			 v.find(*graph_itr) == v.end()){
			visit(s,graph,*graph_itr,H);
		}
	}
}

void setAssociation(Sensor &s, int index, Correlation &ap_y, Correlation &an_y, int MINI_SUP){
	cmpCount++;
	/* initialize */
	std::vector<int> ep = s.getEVOLVING_P();
	std::vector<int> en = s.getEVOLVING_N();

	/* positive */
	if(ap_y.getFLAG()){
		std::vector<int> ptp = ap_y.getTP();
		std::vector<int> ptn = ap_y.getTN();
		std::vector<int> pip,pin;

		set_intersection(ptp.begin(),ptp.end(),ep.begin(),ep.end(),inserter(pip,pip.end()));
		set_intersection(ptn.begin(),ptn.end(),en.begin(),en.end(),inserter(pin,pin.end()));

		if(int(pip.size())+int(pin.size()) >= MINI_SUP){
			ap_y.setFLAG(true);
			ap_y.setTIMESTAMP(pip,pin);
			if(s.getATTRIBUTE() == ap_y.getATTRIBUTE1()){
				ap_y.setSENSORS1(index);
			}else{
				ap_y.setSENSORS2(index);
			}
		}else{
			ap_y.setFLAG(false);
		}
	}

	/* negative */
	if(an_y.getFLAG()){
		std::vector<int> ntp = an_y.getTP();
		std::vector<int> ntn = an_y.getTN();
		std::vector<int> nip,nin;

		if(s.getATTRIBUTE() == an_y.getATTRIBUTE1()){
			set_intersection(ntp.begin(),ntp.end(),ep.begin(),ep.end(),inserter(nip,nip.end()));
			set_intersection(ntn.begin(),ntn.end(),en.begin(),en.end(),inserter(nin,nin.end()));
		}
		if(s.getATTRIBUTE() == an_y.getATTRIBUTE2()){
			set_intersection(ntp.begin(),ntp.end(),en.begin(),en.end(),inserter(nip,nip.end()));
			set_intersection(ntn.begin(),ntn.end(),ep.begin(),ep.end(),inserter(nin,nin.end()));
		}

		if(int(nip.size())+int(nin.size()) >= MINI_SUP){
			an_y.setFLAG(true);
			an_y.setTIMESTAMP(nip,nin);
			if(s.getATTRIBUTE() == an_y.getATTRIBUTE1()){
				an_y.setSENSORS1(index);
			}else{
				an_y.setSENSORS2(index);
			}
		}else{
			an_y.setFLAG(false);
		}
	}
}

bool checkMultiAttribute(std::vector<Sensor> &s, std::vector<int> &x){
	auto x_itr = x.begin();
	std::string a1 = s[*x_itr].getATTRIBUTE();
	x_itr++;
	while(x_itr!=x.end()){
		if(a1 != s[*x_itr].getATTRIBUTE()) return true;
		x_itr++;
	}
	return false;
}

void outputAssociation(std::vector<Sensor> &s, Correlation &a, int N){
	std::vector<int> s1 = a.getSENSORS1();
	std::vector<int> s2 = a.getSENSORS2();
	std::vector<int> timestamp = a.getTIMESTAMP();
	int s1_num = int(s1.size());
	int s2_num = int(s2.size());

	std::string association;
	if(a.getASSOCIATION() == 1) association = "Positive";
	if(a.getASSOCIATION() == -1) association = "Negative";

	// file open
	std::ofstream location("association/"+association+"_"+std::to_string(asCounter)+"_location.txt");
	std::ofstream measurement("association/"+association+"_"+std::to_string(asCounter)+"_measurement.csv");

   location << a.getATTRIBUTE1() << "," << a.getATTRIBUTE2() << std::endl;

	for(int j=0;j<s1_num;j++){
		location << s[s1[j]].getLAT() << ","
						 << s[s1[j]].getLON() << ','
						 << s[s1[j]].getID() << ","
						 << "pin-red" << std::endl;
		}
	for(int j=0;j<s2_num;j++){
		location << s[s2[j]].getLAT() << ","
						 << s[s2[j]].getLON() << ','
						 << s[s2[j]].getID() << ","
						 << "pin-blue" << std::endl;
	}
/*
	for(int n=0;n<N;n++){
		measurement << n;
		for(int m=0;m<s1_num;m++){
			std::vector<double> data = s[s1[m]].getDATA();
			if(data[n] > E){
				measurement << "," << data[n];
			}else{
				measurement << ",";
			}
		}
		for(int m=0;m<s2_num;m++){
			std::vector<double> data = s[s2[m]].getDATA();
			if(data[n] > E){
				measurement << "," << data[n];
			}else{
				measurement << ",";
			}
		}
		measurement << std::endl;
	}
*/
	auto t_itr = timestamp.begin();
	for(int n = 0;n < N;n++){
		measurement << n;
		if(n == *t_itr){
			for(int m=0;m<s1_num;m++){
				std::vector<double> data = s[s1[m]].getDATA();
				measurement << "," << data[n];
			}
			for(int m=0;m<s2_num;m++){
				std::vector<double> data = s[s2[m]].getDATA();
				measurement << "," << data[n];
			}
			t_itr++;
		}else{
			for(int m=0;m<s1_num;m++){
				std::vector<double> data = s[s1[m]].getDATA();
				measurement << ",";
			}
			for(int m=0;m<s2_num;m++){
				std::vector<double> data = s[s2[m]].getDATA();
				measurement << ",";
			}
		}
		measurement << std::endl;
	}
}

void copyAssociation(Correlation ap_x, Correlation an_x, Correlation &ap_y, Correlation &an_y){
	ap_y.setFLAG(ap_x.getFLAG());
	an_y.setFLAG(an_x.getFLAG());
	ap_y.setATTRIBUTE1(ap_x.getATTRIBUTE1());
	ap_y.setATTRIBUTE2(ap_x.getATTRIBUTE2());
	an_y.setATTRIBUTE1(an_x.getATTRIBUTE1());
	an_y.setATTRIBUTE2(an_x.getATTRIBUTE2());
	ap_y.setTIMESTAMP(ap_x.getTP(),ap_x.getTN());
	an_y.setTIMESTAMP(an_x.getTP(),an_x.getTN());
	std::vector<int> s1p = ap_x.getSENSORS1();
	std::vector<int> s2p = ap_x.getSENSORS2();
	std::vector<int> s1n = an_x.getSENSORS1();
	std::vector<int> s2n = an_x.getSENSORS2();
	for(auto itr=s1p.begin();itr!=s1p.end();itr++){
		ap_y.setSENSORS1(*itr);
	}
	for(auto itr=s2p.begin();itr!=s2p.end();itr++){
		ap_y.setSENSORS2(*itr);
	}
	for(auto itr=s1n.begin();itr!=s1n.end();itr++){
		an_y.setSENSORS1(*itr);
	}
	for(auto itr=s2n.begin();itr!=s2n.end();itr++){
		an_y.setSENSORS2(*itr);
	}
}

std::vector<int> getNeighbors(Cluster &cluster, std::vector<Sensor> &s, sensorSet &x){
	std::vector<int> neighbors;

	/* neighbors of root */
	if(x.member.empty()){
		std::vector<int> c = cluster.getMEMBER();
		for(auto c_itr = c.begin();c_itr != c.end();c_itr++){
			if(s[*c_itr].checkADJMULTI()) neighbors.push_back(*c_itr);
		}
		return neighbors;
	}

	/* neighbors  of |x| == 1 */
	if(int(x.member.size()) == 1){
		std::string a = x.attribute1;
		std::vector<int> adj = s[x.member[0]].getADJ();
		for(auto adj_itr=adj.begin();adj_itr!=adj.end();adj_itr++){
			if(s[*adj_itr].getATTRIBUTE() != a) neighbors.push_back(*adj_itr);
		}
		sort(neighbors.begin(),neighbors.end());
		return neighbors;
	}

	/* neighbors  of |x| > 1 */
	for(auto x_itr=x.member.begin();x_itr!=x.member.end();x_itr++){
		std::vector<int> adj = s[*x_itr].getADJ();
		for(auto adj_itr=adj.begin();adj_itr!=adj.end();adj_itr++){
			if(find(neighbors.begin(),neighbors.end(),*adj_itr) == neighbors.end() && find(x.member.begin(),x.member.end(),*adj_itr) == x.member.end()){
				if(s[*adj_itr].getATTRIBUTE() == x.attribute1 || s[*adj_itr].getATTRIBUTE() == x.attribute2) neighbors.push_back(*adj_itr);
			}
		}
	}
	sort(neighbors.begin(),neighbors.end());
	return neighbors;
}
