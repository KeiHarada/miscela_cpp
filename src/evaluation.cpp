#include "miscela.hpp"

void dft(std::vector<int> &,std::vector<double> &,std::vector<double> &);
void powerSpector(std::vector<int> &, std::vector<double> &, std::vector<double> &,std::string);

void evaluation(std::vector<int> &data, int size, int number){
  std::vector<double> Re;
  std::vector<double> Im;
  std::vector<int> x(size,0);

  for(auto data_itr=data.begin();data_itr!=data.end();data_itr++){
    x[*data_itr] = 1;
  }

  dft(x,Re,Im);
  powerSpector(x,Re,Im,std::to_string(number));
  return;
}

void dft(std::vector<int> &x, std::vector<double> &Re, std::vector<double> &Im){
  int size = x.size();
  for(int j=0;j<size;j++){
    double Re_sum = 0;
    double Im_sum = 0;
    for(int i=0;i<size;++i){
      double tht = 2*M_PI/size*j*i;
      Re_sum += x[i]*cos(tht);
      Im_sum += x[i]*sin(tht);
    }
    Re.push_back(Re_sum);
    Im.push_back(Im_sum);
    }
    return;
}

void powerSpector(std::vector<int> &x, std::vector<double> &Re, std::vector<double> &Im, std::string number){
  std::string path = "C:\\Users\\Kei Harada\\Documents\\workspace\\MISCELA\\evaluation\\";
  std::ofstream data(path+number+"_data.csv");
  std::ofstream ps(path+number+"_powerSpector.csv");
  int size = x.size();
  for(int t=0;t<size/2;t++){
    data << t << "," << x[t] << std::endl;
    ps << t << "," << (Re[t]*Re[t]+Im[t]+Im[t])/(Re[0]*Re[0]+Im[0]+Im[0]) << std::endl;
  }
  return;
}
