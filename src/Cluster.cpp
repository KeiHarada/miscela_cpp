// ////////////////////////////////////////////////////////////////////////////////////////////////
//
// functions of the class "Cluster"
//
// ////////////////////////////////////////////////////////////////////////////////////////////////
#include "miscela.hpp"
// ////////////////////////////////////////////////////////////////////////////////////////////////
// constructor //
Cluster::Cluster(){ multiAttribute = false; }
// ////////////////////////////////////////////////////////////////////////////////////////////////
// accessor //
std::vector<int> Cluster::getMEMBER(){ return member; }
bool Cluster::checkMULTI(){ return multiAttribute; }
// ////////////////////////////////////////////////////////////////////////////////////////////////
// setter //
void Cluster::setCLUSTER(int i){ member.push_back(i); }
void Cluster::setMULTI(){ multiAttribute = true; }
// ////////////////////////////////////////////////////////////////////////////////////////////////