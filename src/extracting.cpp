// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// extractingDataInterval()
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
#include "miscela.hpp"
// ////////////////////////////////////////////////////////////////////////////////////////////////
void extractingDataInterval(std::vector<Sensor> &s, int N){
	int size = int(s.size());
	for(int m=0;m<size;m++){
		std::vector<double> data = s[m].getDATA();
		auto data_itr = data.begin();
		int start = -1;
		double prev = E;
		while(data_itr != data.end()){

			/* set the begins */
			if(fabs(*data_itr-E) > 0.01 && fabs(prev-E) < 0.01){
				start = distance(data.begin(),data_itr);
			}

			/* set the ends */
			if(fabs(*data_itr-E) < 0.01 && fabs(prev-E) > 0.01){
				s[m].setINTERVAL(start,distance(data.begin(),data_itr)-1);
				start = -1;
			}
			prev = *data_itr;
			data_itr++;

			/* set final interval */
			if(data_itr == data.end() && start != -1){
				s[m].setINTERVAL(start,N-1);
			}
		}
      //s[m].setCOMPENSATE(N);
      //s[m].setMOVAVG(N);
	}

	return;
}
