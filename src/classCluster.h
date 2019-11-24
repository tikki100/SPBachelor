#pragma once

#include "structPixels.h"
#include "classEdge.h"
#include <iostream>
#include <vector>
#include <set>

namespace Eng
{
/**
  * \class Cluster
  * A cluster belonging to the HPA algorithm
  */	
class Cluster{
public:
	/**
	 *Creates a cluster
	 */
	Cluster();
	/**
	 *Destroys a cluster
	 */
	virtual ~Cluster();

	/**
	 *Gets the upper, left pixel defining the border of the cluster
	 *\return A pixel containing the coordinates of the border.
	 */
	Pixel GetMin() const { return this->m_minBound; };
	/**
	 *Gets the lower right pixel defining the border of the cluster
	 *\return A pixel containing the coordinates of the border.
	 */
	Pixel GetMax() const { return this->m_maxBound; };

	/**
	 *Sets the upper, left pixel defining the border of the cluster
	 *\param coords A pixel containing the coordinates of the border.
	 */
	void SetMin(Pixel& coords) { this->m_minBound = coords; };
	/**
	 *Sets the lower, right pixel defining the border of the cluster
	 *\param coords A pixel containing the coordinates of the border.
	 */
	void SetMax(Pixel& coords) { this->m_maxBound = coords; };

	/**
	 *Test if this cluster contains another cluster.
	 *\param other The cluster to be tested.
	 *\return True if the cluster contains the other cluster (it's within it's border) and false otherwise.
	 */
	bool Contains(Cluster& other);
	/**
	 *Test if this cluster contains a pixel
	 *\param coords The pixel to be tested
	 *\return True if the cluster contains the pixel (it's within it's border) and false otherwise.
	 */
	bool Contains(Pixel& coords);

	std::map<Pixel, std::vector<Edge>> trans; ///Pixels of transitions at the edge.  
	
private:
	Pixel m_minBound; ///The pixel containing the upper, left border pixel.
	Pixel m_maxBound; ///The pixel containing the lower, right border pixel.

	std::vector<Cluster> Clusters; ///A vector containing all clusters inside this cluster.

}; //End of class Cluster

	inline std::ostream& operator<<(std::ostream& os, const Cluster& rhs)
	{
	    os << "Min: " << rhs.GetMin() << " Max: " << rhs.GetMax();
	    return os;
	}

} //End of namespace Eng

