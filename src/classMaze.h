#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <algorithm>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


#include "structPixels.h"
#include "classHPAMaze.h"
#include "classTimer.h"

namespace Eng
{
/**
  * \class Maze
  * An abstract class which can use different algorithms on pictures. 
  */	
class Maze{

public:
	/**
	 * Initilizes a maze, based on an image file.
	 * \param imgFile A pointer to an image file that has been loaded in CImg.
	 * \param name The name of the picture
	 */
	Maze(CImg<unsigned char> * imgFile, std::string name);

	/**
	 *Destroyes the current instance of the maze
	 */
	virtual ~Maze();

	/**
	 *Get the start value for x
	 *\return An unsigned integer for the starting X-coordinate
	 */
	unsigned int GetStartX() { return this->m_Sx; };
	/**
	 *Get the start value for y
	 *\return An unsigned integer for the starting y-coordinate
	 */
	unsigned int GetStartY() { return this->m_Sy; };
	/**
	 *Gets the starting pixel
	 *\return A pixel containing the coordinates for the starting point.
	 */
	Pixel GetStart() { return this->m_Start; };

	/**
	 *Get the end value for x
	 *\return An unsigned integer for the ending X-coordinate
	 */
	unsigned int GetEndX() { return this->m_Ex; };
	/**
	 *Get the end value for y
	 *\return An unsigned integer for the ending Y-coordinate
	 */
	unsigned int GetEndY() { return this->m_Ey; };
	/**
	 *Gets the end pixel
	 *\return A pixel containing the coordinates for the end point.
	 */
	Pixel GetEnd() { return this->m_End; };

	/**
	 *Tests if a pixel is a wall or not.
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\return True if the pixel is walkable, false if it's a wall
	 */
	bool IsWalkable(unsigned int x, unsigned int y);
    /**
	 *Tests if a pixel is a wall or not.
	 *\param coords A pixel to be tested
	 *\return True if the pixel is walkable, false if it's a wall
	 */
	bool IsWalkable(Pixel coords);

	/**
	 *Tests if a pixel is a wall or not.
	 *\param coords A weighted pixel to be tested
	 *\return True if the pixel is walkable, false if it's a wall
	 */
	bool IsWalkable(WeightedPixel coords);
	/**
	 *Tests if a pixel is a specific color
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\param red A char value between 0-255 for the red pixel
	 *\param green A char value between 0-255 for the green pixel
	 *\param blue A char value between 0-255 for the blue pixel
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);

	/**
	 *Tests if a pixel is a specific color
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\param color A RGB color to be tested.
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(unsigned int x, unsigned int y, RGB& color);
	/**
	 *Tests if a pixel is a specific color
	 *\param coords A pixel to be tested
	 *\param red A char value between 0-255 for the red pixel
	 *\param green A char value between 0-255 for the green pixel
	 *\param blue A char value between 0-255 for the blue pixel
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(Pixel& coords, unsigned char r, unsigned char g, unsigned char b);
	/**
	 *Tests if a pixel is a specific color
	 *\param coords A pixel to be tested
	 *\param color A RGB color to be tested.
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(Pixel& coords, RGB& color);

	/**
	 *Gets a color at a specific location
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\return color An RGB containing the color of the pixel at the location
	 */
	const RGB GetColor(unsigned int x, unsigned int y);

	/**
	 *Gets a color at a specific location
	 *\param coords A pixel to be tested
	 *\return color An RGB containing the color of the pixel at the location
	 */
	const RGB GetColor(Pixel& coords);

	/**
	 *Gets a color at a specific location
	 *\param coords A weighted pixel to be tested
	 *\return color An RGB containing the color of the pixel at the location
	 */
	const RGB GetColor(WeightedPixel& coords);

	/**
	 * Runs all shortest path algorithms
	 * Runs Breadth first, A*, HPA* and JPS.
	 */
	void RunAll();

	/**
	 * Runs Breadth First Search for the shortest path on the current maze.
	 * \param display If true, renders the picture on screen.
	 * \param scalar Skips said amount of rendering iterations. Usefull for larger pictures.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunBreadth(bool display = false, unsigned int scalar = 1, bool saveResult = false);

	/**
	 * Runs Dijsktra shortest path algorithm on the current maze.
	 * \param display If true, renders the picture on screen.
	 * \param scalar Skips said amount of rendering iterations. Usefull for larger pictures.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunDijkstra(bool display = false, unsigned int scalar = 1, bool saveResult = false);
	/**
	 * Runs A* shortest path algorithm on the current maze.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunAStar(bool display = false, unsigned int scalar = 1, bool saveResult = false);
	/**
	 * Runs HPA* shortest path algorithm on the current maze.
	 * \param clusterSize The size of each cluster to be created
	 * \param The amount of abstract levels to create. Default is 1.
	 * \param display If true, renders the picture on screen.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunHPAStar(unsigned int clusterSize, unsigned int lvls = 1, bool display = false, bool saveResult = false);
	/**
	 * Runs JPS* shortest path algorithm on the current maze.
	 * \param display If true, renders the picture on screen.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunJPS(bool display = false, unsigned int scalar = 1, bool saveResult = false);
	/**
	 * Tells the program that we are running tests now - and that it should save in the tests folder.
	 */
	void RunningTests() { this-> m_RunningTests = true; };

	void Test();



private:
	CImg<unsigned char> *img;
	/**
	 *Verifies that the maze is valid
	 * Verifies that theres a starting {0, 255, 0} - and an endpoint {0, 0, 255}, as well as 3 channels (RGB). 
	 */
	void Verify();

	/**
	 * Finds the starting {0, 255, 0} and ending {0, 0, 255} coordinates.
	 * \return True if it has found one- and just one starting and end point. Crashes if it finds more.
	 */
	bool FindPoints();

	/**
	 * Runs a step on Breadth First shortest path.
	 * \param queue Takes a queue of pixels that is not empty.
	 * \param came_from Takes a map of pixels, with a key using a pixel. 
 	 * \param cost_so_far Takes a map of doubles, with a key that is a pixel
	 */
	void BreadthStep(std::queue<Pixel>& queue,
		             	std::unordered_map<Pixel, Pixel>& came_from,
	                    std::unordered_map< Pixel, double>& cost_so_far);

	/**
	 * Runs a step on Dijkstra shortest path.
	 * \param queue Takes a queue of weighted pixels that are not empty.
	 * \param came_from Takes a map of pixels with a key that is a pixel.
	 * \param cost_so_far Takes a map of doubles, with a key that is a pixel
	 */
	void DijkstraStep(std::priority_queue<WeightedPixel>& queue, 
	                   std::unordered_map< Pixel, Pixel >& came_from,
	                   std::unordered_map< Pixel, double>& cost_so_far);
	/**
	 * Runs a step on A-star shortest path.
	 * \param queue Takes a queue of weighted pixels that are not empty.
	 * \param came_from Takes a map of pixels with a key that is a pixel.
	 * \param cost_so_far Takes a map of doubles, with a key that is a pixel
	 */
	void AStarStep(std::priority_queue<WeightedPixel>& queue, 
	                   std::unordered_map< Pixel, Pixel >& came_from,
	                   std::unordered_map< Pixel, double>& cost_so_far);

	/**
	 * Runs a step on JPS shortest parth
	 * \param queue Takes a queue of weighted pixels that are not empty.
	 * \param came_from Takes a map of pixels with a key that is a pixel.
	 * \param cost_so_far Takes a map of doubles, with a key that is a pixel
	 * \param visited Takes a map of bools, with a key that is a pixel. 
	 */
	void JPSStep(std::priority_queue<WeightedPixel>& queue, 
	                   std::unordered_map< Pixel, Pixel >& came_from,
	                   std::unordered_map< Pixel, double>& cost_so_far,
	                   std::unordered_map< Pixel, int>& visited);

	/**
	 * Finds all walkable neighbors for a given pixel.
	 * \param coords A pixel which we want the neighbors from.
	 * \param FindEightNeighbors A bool which determines whether to return 8 (true) neighbors or 4 (false) neighbors.
	 * \returns A vector of maximum length 8 containing the neighboring pixels. Can be empty.  
	 */
	std::vector<Pixel> GetNeighbors(Pixel& coords, bool FindEightNeighbors = true);

	/**
	 * Prunes a list of walkable neighbors in a given direction.
	 * \param current The pixel which neighbors we want.
	 * \param came_from The map which contains all came_from values.
	 * \warning Can return unwalkable neighbours!
	 * \returns A vector of maximum length 8 containing the neighboring pixels.
	 */
	std::vector<Pixel> JPSPrunedNeighbors(Pixel current, std::unordered_map< Pixel, Pixel >& came_from);
	/**
	 * Take a step in the direction of dx dy
	 * \param current The currnet pixel which we are examining
	 * \param dx The direction we are walking along the x-axis. Must be between -1 and 1
	 * \param dy The direction we are walking along the y-axis. Must be between -1 and 1
	 * \param came_from Takes a map of pixels with a key that is a pixel.
	 * \param cost_so_far Takes a map of doubles, with a key that is a pixel
	 * \param visited Takes a map of bools, with a key that is a pixel. 
	 */
	std::tuple<Pixel, bool> JPSJump(Pixel& current, 
										int dx, 
										int dy, 
										std::unordered_map< Pixel, Pixel >& came_from,
										std::unordered_map< Pixel, double >& cost_so_far,
										std::unordered_map< Pixel, int >& visited);

	/**
	 * Finds all walkable neighbors for a given pixel.
	 * \param x The x-coordinate of the pixel
	 * \param y The y-coordinate of the pixel
	 * \param FindEightNeighbors A bool which determines whether to return 8 (true) neighbors or 4 (false) neighbors.
	  * \returns A vector of maximum length 8 containing the neighboring pixels. Can be empty.  
	 */
	std::vector<Pixel> GetNeighbors(unsigned int x, unsigned int y, bool FindEightNeighbors = true);


	/**
	 *Color a pixel at a location
	 *\param coords A pixel to be colored.
	 *\param color The RGB color that is used.
	 */
	void ColorPixel(Pixel coords, RGB color);
	/**
	 *Color a pixel at a location
	 *\param coords A weighted pixel to be colored.
	 *\param color The RGB color that is used.
	 */
	void ColorPixel(WeightedPixel coords, RGB color);
    /**
	 *Color a pixel at a location
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\param color The RGB color that is used.
	 */
	void ColorPixel(unsigned int x, unsigned int y, RGB color);

	/**
	 * Gets the heuristic value between two pixels (Euclidean distance)
	 *\param goal The end pixel
	 *\param current The current pixel being examined
	 *\returns A double with a value representing the heuristic cost. 
	 */
	double GetHeuristicCost(Pixel goal, Pixel current);

	/**
	 * Gets the heuristic value between two pixels (Manhatten Distance)
	 *\param goal The end pixel
	 *\param current The current pixel being examined
	 *\returns A double with a value representing the heuristic cost. 
	 */
	double GetHeuristicManhattenCost(Pixel goal, Pixel current);

	/**
	 * Gets the weighted movement cost between two pixels
	 *\attention Only returns the correct cost if two pixels are within 1 step of each other.
	 *\param neighbor The end pixel
	 *\param current The current pixel being examined
	 *\returns A double with a value representing the weighted cost. 
	 */
	double GetWeightedCost(Pixel neighbor, Pixel current);
	/**
	 * Colors all entrances for the clusters in a given maze at the given lvl
	 *\param hpamaze An instantiated version of HPAMaze
	 *\param color The color that the entrances should be
	 *\param lvl The level at which the clusters are.
	 */
	void ColorClusterEntrances(HPAMaze& hpamaze, RGB& color, unsigned int lvl);

	/**
	 * Colors the intra paths for the entrances in a given maze at the given lvl
	 *\param hpamaze An instantiated version of HPAMaze
	 *\param color The color that the entrances should be
	 *\param lvl The level at which the clusters are.
	 */
	void ColorClusterIntraPaths(HPAMaze& hpamaze, RGB& color, unsigned int lvl);

	void ColorHPAPath(HPAMaze& hpamaze, RGB& color, std::unordered_map<Pixel, Pixel>& path);

	/**
	 * Saves the picture in its current state
	 *\param filename The extension which to add to the original file
	 */
	void SavePicture(std::string filename);

	std::vector<Pixel> m_jpsPoints; 


	unsigned int m_Sx;
	unsigned int m_Sy;
	Pixel m_Start; ///A pixel containing the starting coordinates of our starting point. 


	unsigned int m_Ex;
	unsigned int m_Ey;
	Pixel m_End; ///A pixel containing the end coordinates of our goal point.

	bool m_startFound;
	bool m_endFound;

	bool m_RunningTests = false;

	std::string m_name; ///The name of the file being run. 

	inline static const std::string exampleFolder = "../../examples/";
	inline static const std::string testsFolder = "../../tests/";

	inline static const double SQRT2 = sqrt(2.0f);
};
} //End of namespace Eng
