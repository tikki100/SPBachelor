#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include <iostream>
#include <vector>
#include <algorithm>

#include "structPixels.h"
#include "classCluster.h"
#include "classEdge.h"

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
	 * \param maxLevel The maximum level of abstractions
	 * \param clusterSize The size of each cluster
	 */
	HPAMaze(CImg<unsigned char> * imgFile, unsigned int maxLevel, unsigned int clusterSize);

	/**
	 *Destroyes the current instance of the maze
	 */
	virtual ~HPAMaze();

	std::vector<std::vector<Cluster>> GetClusters() { return this->Clusters; };

private:
	std::vector<std::vector<Cluster>> Clusters; ///A vector with each level of the maze, each containing a vector of clusters in that level.

	CImg<unsigned char> *img;

	std::vector<Cluster> BuildClusters(unsigned int level, unsigned int clusterSize, unsigned int width, unsigned int height);

	unsigned int depth;

}; //End of class HPAMaze
} //End of namespace Eng
