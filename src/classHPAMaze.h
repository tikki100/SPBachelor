#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include <iostream>
#include <vector>
#include <algorithm>

#include "structPixels.h"
#include "classCluster.h"

namespace Eng
{
/**
  * \class HPAMaze
  * An abstract class which does a lot of the work for the HPA algorithm.
  */	
class HPAMaze{
public:
	/**
	 * Initilizes an HPA Maze.
	 * \param imgFile A pointer to an image file that has been loaded in CImg.
	 * \param maxLevel The maximum level of abstractions.
	 * \param clusterSize The size of each cluster in pixels. Fx would 10 create clusters of size 10 by 10.
	 */
	HPAMaze(CImg<unsigned char> * imgFile, unsigned int maxLevel, unsigned int clusterSize);

	/**
	 *Destroyes the current instance of the maze
	 */
	virtual ~HPAMaze();

	/**
	 *A definition for the location for clusters.
	 */
	enum class LocCluster { NONE,
					RIGHT,
				    BELOW,
				    LEFT,
				    ABOVE
				    };
	/**
	 * A vector containing each levels vector of clusters. The first object is level 0, the second is level 1 and so on. 
	 */
	std::vector<std::vector<Cluster>> GetClusters() { return this->Clusters; };
	/**
	 * Tests whether or not two clusters are adjacent.
	 * \param c1 The first cluster to be tested
	 * \param c2 The second cluster to be tested
	 * \return 0 if not adjacent, 1 if to the right, 2 if below, 3 to the left, 4 is above.
	 */
	LocCluster IsClusterAdjacent(Cluster& c1, Cluster& c2);

	void Test();

private:
	std::vector<std::vector<Cluster>> Clusters; ///A vector with each level of the maze, each containing a vector of clusters in that level.

	CImg<unsigned char> *img; ///The main maze that we're building from.

	/**
	 * Builds clusters for our given main maze.
	 * \param level The level we're currently building a cluster for.
	 * \param clusterSize The size of the clusters we're building on the current level. 
 	 * \param width The amount of clusters on the horizontal axis that we are gonna build.
 	 * \param height The amount of clusters on the vertical axis that we are gonna build. 
	 * \return True if adjacent, false otherwise.
	 */
	std::vector<Cluster> BuildClusters(unsigned int level, unsigned int clusterSize, unsigned int width, unsigned int height);

	/**
	 * Builds entrances between two clusters
	 * \param c1
	 * \param c2
 	 * \param loc
	 */
	void CreateFirstBorderEntrances(Cluster& c1, Cluster& c2, LocCluster loc);

	void CreateAbstractBorderEntrances(Cluster& c1, Cluster& c2, LocCluster loc);



	bool IsWalkable(Pixel p);

	const RGB GetColor(Pixel p);



	unsigned int depth; ///

}; //End of class HPAMaze

} //End of namespace Eng
