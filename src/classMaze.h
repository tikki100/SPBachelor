#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;
#include <iostream>
#include <array>
#include <queue>
#include <map>

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
	Maze(CImg<unsigned char> * imgFile);

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
	 * Runs Breadth first, Dijkstra, A*, HPA* and JPS.
	 */
	void RunAll();

	/**
	 * Runs Breadth First Search for the shortest path on the current maze.
	 */
	void RunBreadth(bool display = false);
	/**
	 * Runs Dijsktra shortest path algorithm on the current maze.
	 */
	void RunDijkstra();
	/**
	 * Runs A* shortest path algorithm on the current maze.
	 */
	void RunAStar();
	/**
	 * Runs HPA* shortest path algorithm on the current maze.
	 */
	void RunHPAStar();
	/**
	 * Runs JPS* shortest path algorithm on the current maze.
	 */
	void RunJPS();

	void Test();



private:
	CImg<unsigned char> *img;
	CImg<unsigned char> *imgBackup;
	/**
	 *Verifies that the maze is valid
	 * Verifies that theres a starting - and an endpoint, as well as 3 channels (RGB). 
	 */
	void Verify();

	/**
	 * Finds the starting and ending coordinates.
	 * \return True if it has found one- and just one starting and end point. Crashes if it finds more.
	 */
	bool FindPoints();

	/**
	 * Runs a step on Breadth First shortest path.
	 */
	void BreadthStep(std::queue<std::array<unsigned int, 2>>& queue,
		             std::map<std::array<unsigned int, 2>, std::array<unsigned int, 2>>& came_from);

	/**
	 *Color a pixel at a location
	 *\param coords An unsigned int array with an x- and y coordinate in the following order: {x,y}
	 *\param color A const unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 */
	void ColorPixel(std::array<unsigned int,2> coords, std::array<unsigned char, 3> color);
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
};
} //End of namespace Eng
