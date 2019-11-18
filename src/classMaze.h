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
     * A structure to represent pixels
     */
	typedef struct Pixel{
		unsigned int x; ///The x-coordinate
		unsigned int y; ///The y-coordinate
		bool operator ==(const Pixel& rhs) const
		{
			return (this->x == rhs.x && this->y == rhs.y);
		};

		bool operator !=(const Pixel& rhs) const
		{
			return (this->x != rhs.x || this->y != rhs.y);
		};

		bool operator <(const Pixel& rhs) const
		{
			return std::tie(this->x, this->y) < std::tie(rhs.x, rhs.y);
		}
	} Pixel;
	/**
     * A structure to represent weighted pixels
     */
	typedef struct WeightedPixel{
		unsigned int x; ///The x-coordinate
		unsigned int y; ///The y-coordinate
		float w; ///The weight
		bool operator ==(const WeightedPixel& rhs) const
		{
			return (this->x == rhs.x && this->y == rhs.y && this->w == rhs.w);
		};
		bool operator !=(const WeightedPixel& rhs) const
		{
			return (this->x != rhs.x || this->y != rhs.y || this->w != rhs.w);
		};

		bool operator <(const WeightedPixel& rhs) const
		{
			return rhs.w < this->w;
		}
	} WeightedPixel;


	/**
     * A structure to represent colors
     */
	typedef struct RGB{
		unsigned char R;
		unsigned char G;
		unsigned char B;
		bool operator ==(const RGB& rhs) const
		{
			return (this->R == rhs.R && this->G == rhs.G && this->B == rhs.B);
		};
	} RGB;

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
	 *\param coords A pixel to be tested
	 *\param color A RGB color to be tested.
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(Pixel coords, RGB& color);
    /**
	 *Tests if a pixel is a specific color
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\param color A RGB color to be tested.
	 *\return True if the pixel is the color, false otherwise
	 */
	bool IsColor(unsigned int x, unsigned int y, RGB& color);

	/**
	 *Gets a color at a specific location
	 *\param x The x-coordinate of the pixel
	 *\param y The y-coordinate of the pixel
	 *\return color An unsigned char array of length 3 containing the colors in the following order: {r, g, b}
	 */
	const RGB GetColor(unsigned int x, unsigned int y);

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
	 * \param queue Takes a queue of pixels that is not empty.
	 * \param came_from Takes a map of pixels, with a key using a pixel. 
	 */
	void BreadthStep(std::queue<Pixel>& queue,
		             	std::map<Pixel, Pixel>& came_from);

	/**
	 * Runs a step on Dijkstra shortest path.
	 * \param queue Takes a queue of weighted pixels that are not empty.
	 * \param came_from Takes a map of pixels with a key that is a pixel.
	 * \param cost_so_far Takes a map of floats, with a key that is a pixel
	 */
	void DijkstraStep(std::priority_queue<WeightedPixel>& queue, 
	                   std::map< Pixel, Pixel >& came_from,
	                   std::map< Pixel, float>& cost_so_far);
	/**
	 * Runs a step on A-star shortest path.
	 * \param queue Takes a queue of weighted pixels that are not empty.
	 * \param came_from Takes a map of pixels with a key that is a pixel.
	 * \param cost_so_far Takes a map of floats, with a key that is a pixel
	 */
	void AStarStep(std::priority_queue<WeightedPixel>& queue, 
	                   std::map< Pixel, Pixel >& came_from,
	                   std::map< Pixel, float>& cost_so_far);

	/**
	 * Finds all walkable neighbors for a given pixel.
	 * \param coords A pixel which we want the neighbors from.
	 * \param FindEightNeighbors A bool which determines whether to return 8 (true) neighbors or 4 (false) neighbors.
	 * \returns A vector of maximum length 8 containing the neighboring pixels. Can be empty.  
	 */
	std::vector<Pixel> GetNeighbors(Pixel& coords, bool FindEightNeighbors = true);
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
	 * Gets the heuristic value between two pixels
	 *\param goal The end pixel
	 *\param current The current pixel being examined
	 *\returns An unsigned int with a value representing the heuristic cost. 
	 */
	float GetHeuristicCost(Pixel goal, Pixel current);

	/**
	 * Gets the weighted movement cost between two pixels
	 *\attention Only returns the correct cost if two pixels are within 1 step of each other.
	 *\param neighbor The end pixel
	 *\param current The current pixel being examined
	 *\returns A float with a value representing the weighted cost. 
	 */
	float GetWeightedCost(Pixel neighbor, Pixel current);


	unsigned int m_Sx;
	unsigned int m_Sy;
	Pixel m_Start;


	unsigned int m_Ex;
	unsigned int m_Ey;
	Pixel m_End;

	bool m_startFound;
	bool m_endFound;

	std::string m_name;

	inline static const std::string exampleFolder = "../../examples/";
};
} //End of namespace Eng
