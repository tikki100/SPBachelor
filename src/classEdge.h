#pragma once

#include "structPixels.h"
#include <iostream>
#include <vector>

namespace Eng
{
/**
  * \class Cluster
  * A cluster belonging to the HPA algorithm
  */	
class Edge{
public:
	/**
	 *Creates an edge
	 */
	Edge(){};
	/**
	 *Destroys an edge
	 */
	virtual ~Edge(){};

	/**
	 *A definition for the different types of edges.
	 */
	enum EdgeType { INTER, ///< An inter edge is two dots between two different clusters.
				    INTRA ///< An intra edge is two dots inside a cluster.
				    };

	EdgeType type; ///The specific type of the instantiated edge. 

	Pixel s; ///The starting pixel
	Pixel e; ///The end pixel
	float w; ///The weight pixel

	void Set(Pixel s, Pixel e, float w, EdgeType t){
		this->s = s;
		this->e = e;
		this->w = w;
		this->type = t;
	}


};//End of class Cluster

	inline std::ostream& operator<<(std::ostream& os, const Edge& rhs)
	{
	    os << "S: " << rhs.s << " E: " << rhs.e << " w:" << rhs.w << " t:" << rhs.type;
	    return os;
	}
}//End of namespace Eng
