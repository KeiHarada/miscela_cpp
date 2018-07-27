#include "miscela.hpp"
#include <optics/optics.hpp>

/* prototype declaring of the functions */
void checkNeighbors(std::vector<Sensor> &, double);
void dbscan(std::vector<Sensor> &,int,int);
double lat2meter(double);
double lon2meter(double);
double dist1(double,double,double,double);
double dist2(double,double,double,double);

double clusteringSensors(std::vector<Cluster> &clusters,std::vector<Sensor> &s){
	int sensorSize = int(s.size());
	const static int M = 540;
	std::vector<optics::Point<double,2>> points; // Point<type,dimension>
	for(int m=0;m<M;m++){
		optics::Point<double,2> p = {lat2meter(s[m].getLAT()),lon2meter(s[m].getLON())};
		points.push_back(p);
	}

	auto reach_dists = optics::compute_reachability_dists<M,double,2>(points,MIN_PTS); // f<size,type,dimension>(points,min_pts)

	std::vector<double> rdList;
	for(int i=0;i<int(reach_dists.size());i++){
		rdList.push_back(reach_dists[i].reach_dist);
	}
/*
   // show reachability distance
	std::ofstream rd("output/rd.csv");
	for(int i=0;i<int(rdList.size());i++){
		rd << i << "," << rdList[i] << std::endl;
	}
	rd.close();
*/

	std::sort(rdList.begin(),rdList.end());
	double max = rdList[int(rdList.size())-1];
	auto itr = std::lower_bound(rdList.begin(),rdList.end(),max/2);
	double H = *itr;
	//double H = 999;

	/* make connected graph */
	checkNeighbors(s,H);

	/* clustering by DBSCAN */
	int clusterIndex = -1;
	for(int i=0;i<sensorSize;i++){
		if(s[i].getCLUSTER() != NOT_CLASSIFIED) continue;
		if(s[i].getPTS_CNT() >= MIN_PTS){
			clusterIndex++;
			dbscan(s,i,clusterIndex);
		}else{
			s[i].setCLUSTER(NOISE);
		}
	}

	clusters.resize(clusterIndex+1);
	for(int i=0;i<sensorSize;i++) {
		if(s[i].getCLUSTER() != NOISE){
			clusters[s[i].getCLUSTER()].setCLUSTER(i);
		}
	}

	for(int i=0;i<int(clusters.size());i++){
		std::vector<int> c_i = clusters[i].getMEMBER();
		int size = c_i.size();
		std::string a = s[c_i[size-1]].getATTRIBUTE();
		for(auto c_itr=c_i.begin();c_itr!=c_i.end();c_itr++){
			if(s[*c_itr].getATTRIBUTE() != a){
				clusters[i].setMULTI();
				break;
			}
		}
	}

	return H;
}

double clusteringSensors(std::vector<Cluster> &clusters,std::vector<Sensor> &s, double h){
	/* make connected graph */
	checkNeighbors(s,h);

	/* clustering by DBSCAN */
	int clusterIndex = -1;
	int sensorSize = int(s.size());
	for(int i=0;i<sensorSize;i++){
		if(s[i].getCLUSTER() != NOT_CLASSIFIED) continue;
		if(s[i].getPTS_CNT() >= MIN_PTS){
			clusterIndex++;
			dbscan(s,i,clusterIndex);
		}else{
			s[i].setCLUSTER(NOISE);
		}
	}

	clusters.resize(clusterIndex+1);
	for(int i=0;i<sensorSize;i++) {
		if(s[i].getCLUSTER() != NOISE){
			clusters[s[i].getCLUSTER()].setCLUSTER(i);
		}
	}

	for(int i=0;i<int(clusters.size());i++){
		std::vector<int> c_i = clusters[i].getMEMBER();
		int size = c_i.size();
		std::string a = s[c_i[size-1]].getATTRIBUTE();
		for(auto c_itr=c_i.begin();c_itr!=c_i.end();c_itr++){
			if(s[*c_itr].getATTRIBUTE() != a){
				clusters[i].setMULTI();
				break;
			}
		}
	}

	return h;
}

void dbscan(std::vector<Sensor> &s, int now, int c){
	s[now].setCLUSTER(c);
	if(s[now].getPTS_CNT() < MIN_PTS) return;
	std::vector<int> adj = s[now].getADJ();
	for(int next=0;next<int(adj.size());next++){
		if(s[adj[next]].getCLUSTER() != NOT_CLASSIFIED) continue;
		dbscan(s,adj[next],c);
	}
}

void checkNeighbors(std::vector<Sensor> &s, double H){
	int sensorSize = int(s.size());
	for(int i=0;i<sensorSize;i++){
		for(int j=i+1;j<sensorSize;j++){
			if(dist2(s[i].getLAT(),s[i].getLON(),s[j].getLAT(),s[j].getLON()) <= H){
				s[i].setPTS(j,s[j].getATTRIBUTE());
				s[j].setPTS(i,s[i].getATTRIBUTE());
			}
		}
	}
}
