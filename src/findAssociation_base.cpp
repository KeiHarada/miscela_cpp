#include "miscela.hpp"

/* global */
static std::map<int,int> v;
static int asCounter = 0;
static int cmpCount = 0;

/* prototype declaring of functions */
void _Search(Cluster &,std::vector<Sensor> &,std::vector<int>,Correlation,Correlation,int,int,double);
bool _checkMultiAttribute(Correlation);
bool _checkParent(std::vector<Sensor> &,std::vector<int> &,std::vector<int>,double);
bool _checkConnection(std::vector<Sensor> &,std::vector<int> &,double);
void _visit(std::vector<Sensor> &,std::vector<int> &,int,double);
void _setAssociation(Sensor &,int,Correlation &, Correlation &,int);
void _copyAssociation(Correlation,Correlation,Correlation &,Correlation &);
void _outputAssociation(std::vector<Sensor> &,Correlation,int);
std::vector<int> _getNeighbors(Cluster &,std::vector<Sensor> &,std::vector<int> &);
double dist1(double,double,double,double);
double dist2(double,double,double,double);

/* main */
std::vector<int> findAssociation_base(std::vector<Cluster> &clusters, std::vector<Sensor> &s, std::string a1, std::string a2,int N, int MINI_SUP, double H){
	/* initial setting */
	Correlation ap(a1,a2,1);
	Correlation an(a1,a2,-1);
	std::vector<int> x;
	asCounter = 0;
	cmpCount = 0;

	/* start search */
	for(int i=0;i<int(clusters.size());i++){
			_Search(clusters[i],s,x,ap,an,N,MINI_SUP,H);
	}
	//std::cout << asCounter << " sensor sets were found" << std::endl;
	//std::cout << "Comparision Count : " << cmpCount << std::endl;

	std::vector<int> result = {cmpCount,asCounter};
	return  result;
}

void _Search(Cluster &c, std::vector<Sensor> &s, std::vector<int> x, Correlation ap_x, Correlation an_x, int N, int MINI_SUP, double H){
	/* output association */
	if(_checkMultiAttribute(ap_x) && ap_x.getFLAG()) asCounter++; //_outputAssociation(s,ap_x,N);
	if(_checkMultiAttribute(an_x) && an_x.getFLAG()) asCounter++; //_outputAssociation(s,an_x,N);

	/* find neighbor nodes */
	std::vector<int> ns = _getNeighbors(c,s,x);

	/* search*/
	for(auto ns_itr=ns.begin();ns_itr!=ns.end();ns_itr++){

		std::vector<int> y = x;
		y.push_back(*ns_itr);
		sort(y.begin(),y.end());

		/* keep searching if x is a parent of y*/
		if(_checkParent(s,x,y,H)){
			Correlation ap_y(ap_x.getATTRIBUTE1(),ap_x.getATTRIBUTE2(),1);
			Correlation an_y(an_x.getATTRIBUTE1(),an_x.getATTRIBUTE2(),-1);

			/* if y has only a node */
			if(int(y.size()) == 1){
				if(s[y[0]].getATTRIBUTE() == ap_y.getATTRIBUTE1()){
					ap_y.setTIMESTAMP(s[y[0]].getEVOLVING_P(),s[y[0]].getEVOLVING_N());
					an_y.setTIMESTAMP(s[y[0]].getEVOLVING_P(),s[y[0]].getEVOLVING_N());
					ap_y.setSENSORS1(y[0]);
					an_y.setSENSORS1(y[0]);
				}
				if(s[y[0]].getATTRIBUTE() == ap_y.getATTRIBUTE2()){
					ap_y.setTIMESTAMP(s[y[0]].getEVOLVING_P(),s[y[0]].getEVOLVING_N());
					an_y.setTIMESTAMP(s[y[0]].getEVOLVING_N(),s[y[0]].getEVOLVING_P());
					ap_y.setSENSORS2(y[0]);
					an_y.setSENSORS2(y[0]);
				}
				if(ap_y.getSUPPORT() >= MINI_SUP) ap_y.setFLAG(true);
				if(an_y.getSUPPORT() >= MINI_SUP) an_y.setFLAG(true);

				/* copy association on x into association ony */
			}else{
				_copyAssociation(ap_x,an_x,ap_y,an_y);
				_setAssociation(s[*ns_itr],*ns_itr,ap_y,an_y,MINI_SUP);
			}

			/* recursive */
			if(ap_y.getFLAG() || an_y.getFLAG()) _Search(c,s,y,ap_y,an_y,N,MINI_SUP,H);
		}
	}
}

bool _checkParent(std::vector<Sensor> &s, std::vector<int> &parent, std::vector<int> child, double H){
	switch(int(parent.size())){
		case 0: if(parent.empty()) return true;
		case 1:
			child.erase(child.begin());
			if(child == parent){
				return true;
			}else{
				return false;
			}
		default:
			/* roll up operation */
			for(auto child_itr=child.begin();child_itr!=child.end();child_itr++){
				int keep = *child_itr;
				child.erase(child_itr);
				/* if X = Y - {s} is still connected and has multi-attribute sensors */
				if(_checkConnection(s,child,H)){
					if(child == parent){
						return true;
					}else{
						return false;
					}
				}else{
					child.insert(child_itr,keep);
				}
			}
	}

	return true;
}

bool _checkConnection(std::vector<Sensor> &s, std::vector<int> &graph, double H){
	v.clear();

	/* start DFS */
	_visit(s,graph,graph[0],H);

	/* check connection */
	for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
		if(v.find(*graph_itr) == v.end()){
			return false;
		}
	}
	return true;
}

void _visit(std::vector<Sensor> &s, std::vector<int> &graph, int start, double H){

	/* DFS algorithm */
	v[start] = 1;
	for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
		if(dist2(s[start].getLAT(),s[start].getLON(),s[*graph_itr].getLAT(),s[*graph_itr].getLON()) <= H &&
			 v.find(*graph_itr) == v.end()){
			_visit(s,graph,*graph_itr,H);
		}
	}
}

void _setAssociation(Sensor &s, int index, Correlation &ap_y, Correlation &an_y, int MINI_SUP){
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
			if(s.getATTRIBUTE() == ap_y.getATTRIBUTE1()) ap_y.setSENSORS1(index);
			if(s.getATTRIBUTE() == ap_y.getATTRIBUTE2()) ap_y.setSENSORS2(index);
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
			if(s.getATTRIBUTE() == an_y.getATTRIBUTE1()) an_y.setSENSORS1(index);
			if(s.getATTRIBUTE() == an_y.getATTRIBUTE2()) an_y.setSENSORS2(index);
		}else{
			an_y.setFLAG(false);
		}
	}
}

bool _checkMultiAttribute(Correlation a){
	std::vector<int> s1 = a.getSENSORS1();
	std::vector<int> s2 = a.getSENSORS2();
	if(int(s1.size())>0 && int(s2.size())>0) return true;
	return false;
}

void _outputAssociation(std::vector<Sensor> &s, Correlation a, int N){

	std::vector<int> s1 = a.getSENSORS1();
	std::vector<int> s2 = a.getSENSORS2();
	int s1_num = int(s1.size());
	int s2_num = int(s2.size());

	std::string association;
	if(a.getASSOCIATION() == 1) association = "Positive";
	if(a.getASSOCIATION() == -1) association = "Negative";

	/* file open */
	std::ofstream location("association/"+association+"_"+std::to_string(asCounter)+"_location.txt");
	std::ofstream measurement("association/"+association+"_"+std::to_string(asCounter)+"_measurement.csv");

	/* display */
	std::cout << association << " ( k = " << s1_num+s2_num << " )" << "support = " << a.getSUPPORT() << std::endl;
	for(int j=0;j<s1_num;j++){
		std::cout << "| s" << s[s1[j]].getID() << " ( " << s[s1[j]].getATTRIBUTE() << " ) " << std::endl;
		location << s[s1[j]].getLAT() << ","
						 << s[s1[j]].getLON() << ','
						 << s[s1[j]].getID() << ","
						 << "pin-red" << std::endl;
	}
	for(int j=0;j<s2_num;j++){
		std::cout << "| s" << s[s2[j]].getID() << " ( " <<  s[s2[j]].getATTRIBUTE() << " ) " << std::endl;
		location << s[s2[j]].getLAT() << ","
						 << s[s2[j]].getLON() << ','
						 << s[s2[j]].getID() << ","
						 << "pin-blue" << std::endl;
	}

	for(int n = 0;n < N;n++){
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
	std::cout << "| -------------------------------" << std::endl;
}

void _copyAssociation(Correlation ap_x, Correlation an_x, Correlation &ap_y, Correlation &an_y){
	ap_y.setFLAG(ap_x.getFLAG());
	an_y.setFLAG(an_x.getFLAG());
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

std::vector<int> _getNeighbors(Cluster &cluster, std::vector<Sensor> &s, std::vector<int> &x){
	std::vector<int> neighbors;

	/* neighbors of root */
	if(x.empty()){
		std::vector<int> c = cluster.getMEMBER();
		for(auto c_itr=c.begin();c_itr!=c.end();c_itr++){
			neighbors.push_back(*c_itr);
		}

		/* neighbors  of k >= 1*/
	}else{
		for(auto x_itr=x.begin();x_itr!=x.end();x_itr++){
			std::vector<int> adj = s[*x_itr].getADJ();
			for(auto adj_itr=adj.begin();adj_itr!=adj.end();adj_itr++){
				if(find(neighbors.begin(),neighbors.end(),*adj_itr) == neighbors.end() && find(x.begin(),x.end(),*adj_itr) == x.end()){
					neighbors.push_back(*adj_itr);
				}
			}
		}
	}
	sort(neighbors.begin(),neighbors.end());
	return neighbors;
}
