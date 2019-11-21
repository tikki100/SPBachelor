#include "classCluster.h"

namespace Eng
{
	Cluster::Cluster(){}
	Cluster::~Cluster(){}

	bool Cluster::Contains(Cluster& other){
		return false;
	}

	bool Cluster::Contains(Pixel& coords){
		return false;
	}
} //End of namespace Eng
