#pragma once

#include "structPixels.h"
#include <iostream>
#include <vector>

#include <unordered_map>

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

	std::unordered_map<Pixel, Pixel> path; ///Stores the path - only used for INTRA edges.


};//End of class Cluster

	inline std::ostream& operator<<(std::ostream& os, const Edge& rhs)
	{
	    os << "S: " << rhs.s << " E: " << rhs.e << " w:" << rhs.w << " t:" << rhs.type;
	    return os;
	}
}//End of namespace Eng


namespace std {
    template <>
    struct hash<Eng::Edge>
    {
        size_t operator()( const Eng::Edge& k ) const
        {
            // Compute individual hash values for first and second
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<Eng::Pixel>()( k.s );
            res = res * 31 + hash<Eng::Pixel>()( k.e );
            res = res * 31 + hash<unsigned int>()( k.w );
            res = res * 31 + hash<Eng::Edge::EdgeType>()( k.type );
            return res;
        }
    };

}//End of namespace std

