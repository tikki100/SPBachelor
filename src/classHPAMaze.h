#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

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
	 * Initilizes an HPA Maze which creates clusters for the given image. 
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
	 * \return LocCluster::NONE if not adjacent, LocCluster::RIGHT if to the right, LocCluster::BELOW if below, LocCluster::LEFT to the left, LocCluster::ABOVE if above.
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
	 * Builds entrances between two clusters and saves it to this->Clusters.
	 * \param c1 Our first cluster that we want to build entrances from.
	 * \param c2 Our second cluster that we are building entrances to.
 	 * \param loc Where c2 is located compared to c1. 
	 */
	void CreateFirstBorderEntrances(Cluster& c1, Cluster& c2, LocCluster loc);

	/**
	 * Builds an edge on either the x- or the y-axis depending on the input.
	 * \param c1 Our first cluster that we want to build edges from.
	 * \param c2 Our second cluster that we are building edges from.
 	 * \param lineSize The size of the entrance.
 	 * \param i The index at which we broke. This is used to backtrack to find the start or the middle of the entrance.
 	 * \param x If true, x is constant. If false, y is constant.
	 */
	void CreateFirstInterEdges(Cluster &c1, Cluster& c2, unsigned int& lineSize, unsigned int i, bool x);

	void CreateIntraEdges(Cluster& c);

	std::vector<Pixel> GetPixelNeighbors(Pixel p, Cluster& c);

	std::map<Pixel, Pixel> GetPath(Cluster& c, Pixel start, Pixel end);

	void CreateAbstractBorderEntrances(Cluster& c1, Cluster& c2, LocCluster loc);



	bool IsWalkable(Pixel p);

	const RGB GetColor(Pixel p);



	unsigned int depth; ///

}; //End of class HPAMaze

} //End of namespace Eng
