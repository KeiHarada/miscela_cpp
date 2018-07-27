#include "miscela.hpp"
#include <optics/optics.hpp>

double lat2meter(double);
double lon2meter(double);

/* global */
const static int M = 551;

double opticsClustering(std::vector<Cluster>  &clusters, std::vector<Sensor> &s){
	std::vector<optics::Point<double,2>> points; // Point<type,dimension>
	for(int m=0;m<int(s.size());m++){
		optics::Point<double,2> p = {lat2meter(s[m].getLAT()),lon2meter(s[m].getLON())};
		points.push_back(p);
	}
	auto reach_dists = optics::compute_reachability_dists<M,double,2>(points,MIN_PTS); // f<size,type,dimension>(points,min_pts)

	std::vector<double> rdList;
	for(int i=0;i<int(reach_dists.size());i++){
		rdList.push_back(reach_dists[i].reach_dist);
	}
	std::sort(rdList.begin(),rdList.end());
	double max = rdList[int(rdList.size())-1];
	auto itr = std::lower_bound(rdList.begin(),rdList.end(),max/2);
	double h = *itr;
	std::cout << max << " " << h << std::endl;

	auto clusters_index = get_cluster_indices(reach_dists,h);

	int count = -1;
	for(int i=0;i<int(clusters_index.size());i++){
		if(clusters_index[i].size() > 1){
			count++;
			std::sort(clusters_index[i].begin(),clusters_index[i].end());
			Cluster c;
			std::string attribute;
			for(int j=0;j<int(clusters_index[i].size());j++){
				if(j == 0){
					attribute = s[clusters_index[i][j]].getATTRIBUTE();
				}else{
					if(s[clusters_index[i][j]].getATTRIBUTE() != attribute) c.setMULTI();
				}
				c.setCLUSTER(clusters_index[i][j]);
			}
			clusters.push_back(c);
		}
	}
	return h;
}