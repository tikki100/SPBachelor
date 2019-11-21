#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include "structPixels.h"

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

private:

}; //End of class HPAMaze
} //End of namespace Eng
