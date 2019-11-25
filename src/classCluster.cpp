#include "classCluster.h"

namespace Eng
{
	Cluster::Cluster(){}
	Cluster::~Cluster(){}

	bool Cluster::Contains(Cluster& other){
		if(this->m_maxBound.x >= other.GetMin().x && other.GetMin().x >= this->m_minBound.x
			&& this->m_maxBound.x >= other.GetMax().x && other.GetMax().x >= this->m_maxBound.x
			&& this->m_maxBound.y >= other.GetMin().y && other.GetMin().y >= this->m_minBound.y
			&& this->m_maxBound.y >= other.GetMax().y && other.GetMax().y >= this->m_minBound.y)
		{
			return true;
		}
		return false; //TODO
	}

	bool Cluster::Contains(Pixel& coords){
		if((this->m_maxBound.x >= coords.x  && coords.x >= this->m_minBound.x) 
			&& (this->m_maxBound.y >= coords.y && coords.y >= this->m_minBound.y))
		{
			return true;
		}
		return false;
	}
} //End of namespace Eng
