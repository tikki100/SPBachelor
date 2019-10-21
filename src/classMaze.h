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
	 */
	Maze(CImg<unsigned char> * imgFile, std::string name);

	/**
	 *Destroyes the current instance of the maze
	 */
	virtual ~Maze();

    /**
	 *Displays the current maze with any alterations.
	 */
	void Display();

	/**
	 *Get the start value for x
	 *\return An integer for the starting X-coordinate
	 */
	int GetStartX();
	/**
	 *Get the start value for y
	 *\return An integer for the starting y-coordinate
	 */
	int GetStartY();

	/**
	 *Get the end value for x
	 *\return An integer for the ending X-coordinate
	 */
	int GetEndX();
	/**
	 *Get the end value for y
	 *\return An integer for the ending Y-coordinate
	 */
	int GetEndY();
	/**
	 *Tests if a pixel is a wall or not.
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\return True if the pixel is walkable, false if it's a wall
	 */
	bool IsWalkable(unsigned int x, unsigned int y);
    /**
	 *Tests if a pixel is a wall or not.
	 *\param coords An unsigned int array which contains an x- and y value in the following order: {x,y}
	 *\return True if the pixel is walkable, false if it's a wall
	 */
	bool IsWalkable(std::array<unsigned int, 2> coords);

	/**
	 *Tests if a pixel is a wall or not.
	 *\param coords An unsigned int array which contains an x- and y value and a weight in the following order: {x,y, w}
	 *\return True if the pixel is walkable, false if it's a wall
	 */
	bool IsWalkable(std::array<unsigned int, 3> coords);
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
	 *\param coords An unsigned int array with an x- and y coordinate in the following order: {x,y}
	 *\param color A const unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(std::array<unsigned int, 2> coords, std::array<unsigned char, 3> color);
    /**
	 *Tests if a pixel is a specific color
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\param color A const unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(unsigned int x, unsigned int y, std::array<unsigned char, 3> color);

	/**
	 *Gets a color at a specific location
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\return color An unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 */
	const std::array<unsigned char, 3> GetColor(unsigned int x, unsigned int y);

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
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunDijkstra(bool display = false, bool saveResult = false);
	/**
	 * Runs A* shortest path algorithm on the current maze.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunAStar(bool display = false, bool saveResult = false);
	/**
	 * Runs HPA* shortest path algorithm on the current maze.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunHPAStar(bool display = false, bool saveResult = false);
	/**
	 * Runs JPS* shortest path algorithm on the current maze.
	 * \param saveResult If display is true, this variable determines whether or not to save the result as a file.
	 */
	void RunJPS(bool display = false, bool saveResult = false);

	void Test();



private:
	CImg<unsigned char> *img;
	CImg<unsigned char> *imgBackup;
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
	 * \param queue Takes a queue of a length 2 unsignd int array, that is not empty.
	 * \param came_from Takes a map of unsigned int arrays with the length 2, with a key that is an unsigned int array of length 2
	 */
	void BreadthStep(std::queue<std::array<unsigned int, 2>>& queue,
		             	std::map<std::array<unsigned int, 2>, std::array<unsigned int, 2>>& came_from);

	/**
	 * Runs a step on Dijkstra shortest path.
	 * \param queue Takes a queue of a length 3 unsignd int array, that is not empty, {x-coordinate, y-coordinate, weight}.
	 * \param came_from Takes a map of unsigned int arrays with the length 2, with a key that is an unsigned int array of length 2
	 * \param cost_so_far Takes a map of unsigned int, with a key that is an unsigned int array of length 2.
	 */
	void DijkstraStep(std::vector<std::array<unsigned int, 3>>& queue, 
	                   std::map< std::array<unsigned int, 2>, std::array<unsigned int, 2> >& came_from,
	                   std::map< std::array<unsigned int, 2>, unsigned int>& cost_so_far);
	/**
	 * Runs a step on A-star shortest path.
	 */
	void AStarStep();

	/**
	 * Finds all walkable neighbors for a given pixel.
	 * \param coords An array of 2 unsigned ints with the coordinates of a pixel in the following order: {x,y}
	 * \returns An unknown length vector containing unsigned int arrays of length 2. 
	 */
	std::vector<std::array<unsigned int,2>> GetNeighbors(std::array<unsigned int, 2> coords);
	/**
	 * Finds all walkable neighbors for a given pixel.
	 * \param x The x-coordinate of the pixel
	 * \param y The y-coordinate of the pixel
	 * \returns An unknown length vector containing unsigned int arrays of length 2. 
	 */
	std::vector<std::array<unsigned int,2>> GetNeighbors(unsigned int x, unsigned int y);


	/**
	 *Color a pixel at a location
	 *\param coords An unsigned int array with an x- and y coordinate in the following order: {x,y}
	 *\param color A const unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 */
	void ColorPixel(std::array<unsigned int,2> coords, std::array<unsigned char, 3> color);
	/**
	 *Color a pixel at a location
	 *\param coords An unsigned int array with an x- and y and a weight coordinate in the following order: {x,y, w}
	 *\param color A const unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 */
	void ColorPixel(std::array<unsigned int,3> coords, std::array<unsigned char, 3> color);
    /**
	 *Color a pixel at a location
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\param color A const unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 */
	void ColorPixel(unsigned int x, unsigned int y, std::array<unsigned char, 3> color);


	unsigned int m_Sx;
	unsigned int m_Sy;

	unsigned int m_Ex;
	unsigned int m_Ey;

	bool m_startFound;
	bool m_endFound;

	std::string m_name;

	inline static const std::string exampleFolder = "../../examples/";
};
} //End of namespace Eng
