#include "classMaze.h"

namespace Eng
{ //Start of Eng

Maze::Maze(CImg<unsigned char> * imgFile, std::string name)
{
	//Saving variables
	this->img = imgFile;
	this->m_name = name;

	//Inital vlaues
	this->m_Sx = 0;
	this->m_Sy = 0;
	this->m_Start = {0,0};

	this->m_Ex = 0;
	this->m_Ey = 0;
	this->m_End = {0,0};

	this->m_startFound = false;
	this->m_endFound = false;

	//Initialize function
	this->Verify();
}

Maze::~Maze()
{

}

void Maze::Verify()
{
	if(this->img->spectrum() != 3)
	{
		std::cout << this->img->spectrum() << std::endl;
		throw std::invalid_argument( "Image does not contain 3 spectrums (RGB profile)" );
	}

	if((this->FindPoints()) == false)
	{
		throw std::invalid_argument( "Image does not contain a green starting point and a blue ending point" );
	}
}

bool Maze::FindPoints()
{
	cimg_forXY(*(this->img), x,y)
    {
        if(this->IsColor(x,y,0,255,0)){
        	if(this->m_startFound == false){
				this->m_Sx = x;
	        	this->m_Sy = y;
	        	this->m_Start = {m_Sx, m_Sy};
	        	this->m_startFound = true;
	        	std::cout << "Found starting pixel at {" << x << "," << y << "}" << std::endl;
        	}
        	else
        	{
        		throw std::invalid_argument( "Image contained more than one starting point" );
        	}
        }
        else if(this->IsColor(x,y,0,0,255)){
        	if(this->m_endFound == false){
	        	this->m_Ex = x;
	        	this->m_Ey = y;
	        	this->m_End = {m_Ex,m_Ey};
	        	this->m_endFound = true;
	        	std::cout << "Found end pixel at {" << x << "," << y << "}" << std::endl;
	       	}
	       	else
	       	{
	       		throw std::invalid_argument( "Image contained more than one ending point" );
	       	}
        }
        //In theory, we could break out early by maximising x and y -
        //but doing so, wouldn't ensure against multiple starting
        //or ending points. 
    }
    return (m_startFound && m_endFound);
}

bool Maze::IsWalkable(WeightedPixel coords)
{
	return this->IsWalkable(coords.x, coords.y);
}

bool Maze::IsWalkable(Pixel coords)
{
	return this->IsWalkable(coords.x, coords.y);
}

bool Maze::IsWalkable(unsigned int x, unsigned int y)
{
	if(this->IsColor(x, y, 255, 255, 255))
	{
		return true;
	}
	return false;
}

bool Maze::IsColor(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	RGB color = {r, g, b};
	return this->IsColor(x, y, color);
}

bool Maze::IsColor(Pixel coords, RGB& color)
{
	return this->IsColor(coords.x, coords.y, color);
}

bool Maze::IsColor(unsigned int x, unsigned int y, RGB& color)
{
	RGB _checkColor = this->GetColor(x,y);

	if(_checkColor == color){
		return true;
	}
	return false;
}

const Maze::RGB Maze::GetColor(unsigned int x, unsigned int y)
{
	return {(*this->img)(x,y, 0), (*this->img)(x,y, 1), (*this->img)(x,y, 2)};
}

void Maze::ColorPixel(WeightedPixel coords, RGB color)
{
	this->ColorPixel(coords.x, coords.y, color);
}

void Maze::ColorPixel(Pixel coords, RGB color)
{
	this->ColorPixel(coords.x, coords.y, color);
}

void Maze::ColorPixel(unsigned int x, unsigned int y, RGB color)
{
	const unsigned char _color[3] = {color.R, color.G, color.B}; //HACK: I imagine that there must be a better way to do this.
	this->img->draw_point(x,y, _color);
}

int Maze::GetStartX(){ return this->m_Sx;}

int Maze::GetStartY(){ return this->m_Sy;}

int Maze::GetEndX(){ return this->m_Ex;}

int Maze::GetEndY(){ return this->m_Ey;}

void Maze::RunBreadth(bool display, unsigned int scalar, bool saveResult)
{
	std::cout << "Running Breadth!" << std::endl;
	std::vector<Pixel> queue;
	std::map<Pixel, Pixel> came_from;

	queue.push_back({this->m_Sx, this->m_Sy});
	Pixel Test = {this->m_Sx, this->m_Sy};
	came_from[Test] = Test;

	
	if(display)
	{
		if(scalar == 0) 
			scalar = 1;
		CImgDisplay main_disp((*this->img),"Breadth First Search", 0);

		Pixel current = {this->m_Ex, this->m_Ey};
		Pixel start = {this->m_Sx, this->m_Sy};

	    while (!main_disp.is_closed()) 
	    {
	    	main_disp.resize(500,500).display((*this->img));
			if(!queue.empty())
			{
				for(unsigned int i = 0; i < 1 * scalar; i++)
				{
					this->BreadthStep(queue, came_from);
					if(queue.empty())
						break;
				}
			}
			else if(queue.empty() && current != start)
			{
				unsigned int cond = (1 * scalar/100);
				if (cond == 0)
					cond = 1;
				for(unsigned int i = 0; i < cond; i++)
				{
					this->ColorPixel(current, {255, 0, 0});
			        current = came_from[current];
			        if(current == start)
			        {
			        	this->ColorPixel(current, {255, 0, 0});
			        	break;
			        }
				}
			}
	    }
	    if(saveResult)
	    {
	    	std::string saveName = this->exampleFolder + this->m_name + "_breadth.png";
	    	this->img->save(saveName.c_str());
	    }
	}

	else
	{
		Pixel current = {this->m_Ex, this->m_Ey};
		Pixel start = {this->m_Sx, this->m_Sy};

		while(!queue.empty())
		{
			this->BreadthStep(queue, came_from);
		}
		while(current != start)
		{
			this->ColorPixel(current, {255, 0, 0});
			current = came_from[current];
		}
		this->ColorPixel(start, {255, 0, 0});

		std::string saveName = this->exampleFolder + this->m_name + "_breadth.png";

		this->img->save(saveName.c_str());
	}
	

}

void Maze::BreadthStep(std::vector<Pixel>& queue, 
	                   	std::map< Pixel, Pixel >& came_from)
{
	RGB grey = {126, 126, 126};
	RGB black = {0, 0, 0};
	RGB path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do BreadthStep on an empty queue." );

	Pixel current = queue.front();
	queue.pop_back();

	std::vector<Pixel> neighbors = this->GetNeighbors(current);

	for(Pixel neighbor : neighbors)
	{

		//std::cout << "Checking: {" << neighbor.x << "," << neighbor.y << "} came from: {" << current.x << "," << current.y << "}" << std::endl;

		if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
		{
			//Break the loop
			std::cout << "Found end!" << std::endl;
			queue.clear();
			came_from.emplace(neighbor, current);
			return;
		}
		else
		{
			if(came_from.count(neighbor) == 0)
			{
				this->ColorPixel(neighbor, grey);
				came_from.emplace(neighbor, current);
				queue.push_back(neighbor);
			}
		}
	}
}

void Maze::RunDijkstra(bool display, unsigned int scalar, bool saveResult)
{
	std::cout << "Running Djikstra!" << std::endl;
	std::vector<WeightedPixel> queue;
	queue.reserve(100);

	std::map<Pixel, Pixel> came_from;
	std::map<Pixel, float> cost_so_far;

	came_from[{this->m_Sx, this->m_Sy}] = {this->m_Sx, this->m_Sy};
	cost_so_far[{this->m_Sx, this->m_Sy}] = 0.0f;

	queue.emplace_back((WeightedPixel){this->m_Sx, this->m_Sy, 0});

	Pixel current = {this->m_Ex, this->m_Ey};
	Pixel start = {this->m_Sx, this->m_Sy};

	if(display)
	{
		CImgDisplay main_disp((*this->img),"Djikstra Search", 0);
		if(scalar == 0) 
			scalar = 1;

	    while (!main_disp.is_closed()) 
	    {
	    	main_disp.resize(500,500).display((*this->img));
			if(!queue.empty())
			{
				for(int i = 0; i < 1*scalar; i++)
				{
					this->DijkstraStep(queue, came_from, cost_so_far);
					if(queue.empty())
						break;
				}
			}
			else if(queue.empty() && current != start)
			{
				unsigned int cond = (1 * scalar/100);
				if (cond == 0)
					cond = 1;
				for(int i = 0; i < 1*cond; i++)
				{
					this->ColorPixel(current, {255, 0, 0});
			        current = came_from[current];
			        if(current == start)
			        {
			        	this->ColorPixel(current, {255, 0, 0});
			        }
			        if(queue.empty() || current == start)
						break;
		    	}
			}
	    }

	    if(saveResult)
	    {
	    	std::string saveName = this->exampleFolder + this->m_name + "_dijkstra.png";
	    	this->img->save(saveName.c_str());
	    }
	}
	else
	{
		while(!queue.empty())
		{
			this->DijkstraStep(queue, came_from, cost_so_far);
		}
		std::cout << "Coloring start" << std::endl;
		while(current != start)
		{
			this->ColorPixel(current, {255, 0, 0});
			current = came_from[current];
		}
		this->ColorPixel(start, {255, 0, 0});

		std::string saveName = this->exampleFolder + this->m_name + "_dijkstra.png";

		this->img->save(saveName.c_str());

	}
}

void Maze::DijkstraStep(std::vector<WeightedPixel>& queue, 
	                   		std::map< Pixel, Pixel >& came_from,
	                   		std::map< Pixel, float>& cost_so_far)
{
	RGB grey = {126, 126, 126};
	RGB path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do DijkstraStep on an empty queue." );

	std::sort(queue.begin(), queue.end(), [this](auto l, auto r) {return (l.w > r.w); });

	WeightedPixel coords = queue.back();
	Pixel current = {coords.x, coords.y};

	queue.pop_back();

	std::vector<Pixel> neighbors = this->GetNeighbors(current);

	for(Pixel neighbor : neighbors)
	{
		//Since the cost between is always 1, we add 1 to the new weight.
		//new_weight = cost_so_far[coords] + cost_between(coords, next)
		float new_weight = cost_so_far[current] + this->GetWeightedCost(neighbor, current);

		//std::cout << "Checking: {" << neighbor.x << "," << neighbor.y << "}" << std::endl;

		if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
		{
			//Break the loop
			std::cout << "Found end!" << std::endl;
			came_from[neighbor] = current;
			cost_so_far[neighbor] = new_weight;
			queue.clear();
			return;
		}
		else
		{
			this->ColorPixel(neighbor, grey);
			if(cost_so_far.count(neighbor) == 0 || new_weight < cost_so_far[neighbor])
			{
				cost_so_far[neighbor] = new_weight;
				WeightedPixel neigh_weighted = {neighbor.x, neighbor.y, new_weight};
				queue.emplace_back(neigh_weighted);
				came_from[neighbor] = current;
			}

		}
	}
}

void Maze::RunAStar(bool display, bool saveResult)
{
	std::cout << "Running A*!" << std::endl;
	std::vector<WeightedPixel> queue;
	queue.reserve(100);

	std::map< Pixel, Pixel> came_from;
	std::map< Pixel, float> cost_so_far;

	came_from[{this->m_Sx, this->m_Sy}] = {this->m_Sx, this->m_Sy};
	cost_so_far[{this->m_Sx, this->m_Sy}] = 0;

	queue.emplace_back((WeightedPixel){this->m_Sx, this->m_Sy, 0});

	Pixel current = {this->m_Ex, this->m_Ey};
	Pixel start = {this->m_Sx, this->m_Sy};

	if(display)
	{
		//TODO
	}
	else
	{
		while(!queue.empty())
		{
			this->AStarStep(queue, came_from, cost_so_far);
		}
	}
}

void Maze::AStarStep(std::vector<WeightedPixel>& queue, 
	                   		std::map< Pixel, Pixel >& came_from,
	                   		std::map< Pixel, float>& cost_so_far)
{
	RGB grey = {126, 126, 126};
	RGB path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do DijkstraStep on an empty queue." );

	std::sort(queue.begin(), queue.end(), [this](auto l, auto r) {return (l.w > r.w); });

	WeightedPixel coords = queue.back();
	Pixel current = {coords.x, coords.y};

	queue.pop_back();

	std::vector<Pixel> neighbors = this->GetNeighbors(current);

	for(Pixel neighbor : neighbors)
	{
		unsigned int new_weight = cost_so_far[current] + 1;

		//std::cout << "Checking: {" << neighbor[0] << "," << neighbor[1] << "}" << std::endl;

		if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
		{
			//Break the loop
			std::cout << "Found end!" << std::endl;
			came_from[neighbor] = current;
			cost_so_far[neighbor] = new_weight;
			queue.clear();
			return;
		}
		else
		{
			this->ColorPixel(neighbor, grey);
			if(cost_so_far.count(neighbor) == 0 || new_weight < cost_so_far[neighbor])
			{
				cost_so_far[neighbor] = new_weight;
				float priority = new_weight + this->GetHeuristicCost(m_End, neighbor);
				WeightedPixel neigh_weighted = {neighbor.x, neighbor.y, priority};
				queue.emplace_back(neigh_weighted);
				came_from[neighbor] = current;
			}

		}
	}
}

float Maze::GetHeuristicCost(Pixel goal, Pixel current)
{
	return 0.0f;
}

float Maze::GetWeightedCost(Pixel neighbor, Pixel current)
{
	/*
	float cost = 0.0f;
	if(neighbor == current)
		return cost;

	if((current.x - neighbor.x == 0) || (current.y - neighbor.y == 0))
		cost = 1.0f;
	else
		cost = 1.41f;*/

	float cost = 1.0f

	return cost;
}


void Maze::Test()
{

}

std::vector<Maze::Pixel> Maze::GetNeighbors(Pixel& coords, bool FindEightNeighbors)
{
	return this->GetNeighbors(coords.x, coords.y, FindEightNeighbors);
}

std::vector<Maze::Pixel> Maze::GetNeighbors(unsigned int x, unsigned int y, bool FindEightNeighbors)
{
	std::vector<Pixel> result;

	if(FindEightNeighbors)
	{
		result.reserve(8);
	
		for(int _x = -1; _x < 2; _x++) //Check all 8 neighboring pixels.
		{
			if(x == 0 && _x == -1) //If we move too far left, skip
				continue;
			else if(x == (this->img->width()-1) && _x == 1) //If we're too  far right, skip
				continue;
			for(int _y = -1; _y < 2; _y++)
			{
				if(y == 0 && _y == -1)  //If we're above the picture, skip
					continue;
				else if(y == (this->img->height()-1) && _y == 1) //If we're below the picture, skip
					continue;
				else if(x == x + _x && y == y + _y) //If we're on the current pixel, skip
					continue;
				Pixel neighbor = {x + _x, y + _y};
	
				if(this->IsWalkable(neighbor))
				{
					result.push_back(neighbor);
				}
				else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
				{
					result.push_back(neighbor);
					return result; //If we have found the end, return early.
				}
			}
		}
	}
	else
	{
		result.reserve(4);	
		Pixel neighbor;

		//An important thing to note is that neighbors are pushed to the back of the vector - aka first in, last out.
		if(x != (this->img->width()-1)) //Down
		{
			neighbor = {x + 1, y};
			if(this->IsWalkable(neighbor))
			{
				result.push_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.push_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
		if(x != 0) //Up
		{
			neighbor = {x - 1, y};
			if(this->IsWalkable(neighbor))
			{
				result.push_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.push_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
		if(y != (this->img->height()-1)) //Right
		{
			neighbor = {x, y+1};
			if(this->IsWalkable(neighbor))
			{
				result.push_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.push_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
		if(y != 0) //Left
		{
			neighbor = {x, y-1};
			if(this->IsWalkable(neighbor))
			{
				result.push_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.push_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
	}

	return result;

}

void Maze::RunAll()
{
	this->RunBreadth();
	this->RunAStar();
}

}//End of namespace Eng
