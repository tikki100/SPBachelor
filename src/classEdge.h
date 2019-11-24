#pragma once

#include "structPixels.h"
#include <iostream>

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
	Edge();
	/**
	 *Destroys an edge
	 */
	virtual ~Edge();

	enum EdgeType { INTER, INTRA };

	EdgeType type; ///The type of the edge. 

	Pixel s; ///The starting pixel
	Pixel e; ///The end pixel
	float w; ///The weight pixel


};//End of class Cluster
}//End of namespace Eng
