#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#include <unordered_map>
#include <map>

#include "structPixels.h"
#include "classCluster.h"

#include "classTimer.h"

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
	HPAMaze(CImg<unsigned char> * imgFile, unsigned int maxLevel, unsigned int clusterSize, Pixel start, Pixel end);

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

	std::unordered_map<Pixel, Pixel> AbstractPathfind(unsigned int lvl);

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
	/**
	 * Builds an edge between the inter edges, if a path exists.
	 * \param c The cluster that we want to build the edges on. 
	 */
	void CreateIntraEdges(Cluster& c);

	void InsertPixelIntoLevel(Pixel p, unsigned int lvl);

	std::vector<Edge> GetInterEdges(Pixel& p, unsigned int lvl);

	std::vector<Edge> GetEdges(Pixel& p, unsigned int lvl);

	/**
	 * Gets all possible neighbors to a pixel in a specific cluster.
	 * \param p The pixel that we are examining
	 * \param c The cluster in which the pixel is located
	 * \return A vector containing the pixels of all neighbors.
	 */
	std::vector<Pixel> GetPixelNeighbors(Pixel p, Cluster& c); 

	/**
	 * Gets the path between two pixels inside a cluster, if such a path exists. 
	 * \param c The cluster in which the pixels are contained.
	 * \param start The first pixel from which we are starting
	 * \param end The second pixel from which we wish to pathfind to.
	 * \return A tuple. The first is a map, containing the path. The second is the weight of the path. 
	 */
	std::tuple<std::unordered_map<Pixel, Pixel>, double> GetPath(Cluster& c, Pixel start, Pixel end);
	//TODO
	void CreateAbstractBorderEntrances(Cluster& c1, Cluster& c2, LocCluster loc);
	/**
	 * Gets the heuristic cost between a goal pixel and our current pixel.
	 * \param goal The pixel which is our goal.
	 * \param current The pixel from which we are examining.
	 * \return A double in which the euclidean distance is contained.
	 */
	double GetHeuristicCost(Pixel goal, Pixel current);
	/**
	 * Tests if we can walk on a pixel.
	 * \param p The pixel which we want to test.
	 * \return True, if we can walk on the pixel. False otherwise.
	 */
	bool IsWalkable(Pixel p);
	/**
	 * Tests the color of the pixel
	 * \param p The pixel which we want to test.
	 * \return An RGB containing the color of the pixel. 
	 */
	const RGB GetColor(Pixel p);

	Pixel goal;
	Pixel start;

	unsigned int depth; ///

	inline static const double SQRT2 = sqrt(2.0f);

}; //End of class HPAMaze

} //End of namespace Eng
