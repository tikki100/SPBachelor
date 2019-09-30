#pragma once

#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;
#include <iostream>

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
	 *Displays the current maze
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
	bool IsWalkable(int x, int y);
	/**
	 *Tests if a pixel is a specific color
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\param red A value between 0-255 for the red pixel
	 *\param green A value between 0-255 for the green pixel
	 *\param blue A value between 0-255 for the blue pixel
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(int x, int y, int red, int green, int blue);

private:
	CImg<unsigned char> *img;
	/**
	 *Verifies that the maze is valid
	 * Verifies that theres a starting - and an endpoint, as well as 3 channels (RGB). 
	 */
	void Verify();

	/**
	 * Finds the starting and ending coordinates.
	 */
	bool FindPoints();

	int Sx;
	int Sy;

	int Ex;
	int Ey;
};
} //End of namespace Eng
