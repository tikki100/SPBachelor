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

	this->m_Ex = 0;
	this->m_Ey = 0;

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

void Maze::Display()
{
	CImg<unsigned char> * result = this->img;
	//Temporary resizing
	/*if(this->img->width() <= 100 && this->img->height() <= 100)
	{
		CImg<unsigned char> scaling = *(this->img);
		if (this->img->width() <= 25 && this->img->height() <= 25)
		{
			if (this->img->width() <= 10 && this->img->height() <= 10)
			{
				scaling.resize(scaling.width()*50, scaling.height()*50);
			}
			else
			{
				scaling.resize(scaling.width()*15, scaling.height()*15);
			}
		}
		else
		{
			scaling.resize(scaling.width()*10, scaling.height()*10);
		}
		result = &scaling;
	}*/

	

	//Updating per step
	CImgDisplay main_disp((*result),"Test image 2", 0);
    while (!main_disp.is_closed()) 
    {
    	/*
    	main_disp.resize(100,100).display((*this->img));
    	std::cout << "Loop: " << y << " Color: " << (int)this->GetColor(0, y)[1] << std::endl;
    	std::array<unsigned char, 3> color = {255, 0, 0};
    	this->ColorPixel(0,y, color);
    	y++;*/
    }
    
}

bool Maze::IsWalkable(std::array<unsigned int, 3> coords)
{
	return this->IsWalkable(coords[0], coords[1]);
}

bool Maze::IsWalkable(std::array<unsigned int, 2> coords)
{
	return this->IsWalkable(coords[0], coords[1]);
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
	std::array<unsigned char, 3> color = {r, g, b};
	return this->IsColor(x, y, color);
}

bool Maze::IsColor(std::array<unsigned int, 2> coords, std::array<unsigned char, 3> color)
{
	return this->IsColor(coords[0], coords[1], color);
}

bool Maze::IsColor(unsigned int x, unsigned int y, std::array<unsigned char, 3> color)
{
	std::array<unsigned char, 3> _checkColor = this->GetColor(x,y); 

	if(_checkColor[0] == color[0]
        && _checkColor[1] == color[1]
        && _checkColor[2] == color[2]){
		return true;
	}
	return false;
}

const std::array<unsigned char, 3> Maze::GetColor(unsigned int x, unsigned int y)
{
	return {(*this->img)(x,y, 0), (*this->img)(x,y, 1), (*this->img)(x,y, 2)};
}

void Maze::ColorPixel(std::array<unsigned int,3> coords, std::array<unsigned char, 3> color)
{
	this->ColorPixel(coords[0], coords[1], color);
}

void Maze::ColorPixel(std::array<unsigned int,2> coords, std::array<unsigned char, 3> color)
{
	this->ColorPixel(coords[0], coords[1], color);
}

void Maze::ColorPixel(unsigned int x, unsigned int y, std::array<unsigned char, 3> color)
{
	const unsigned char _color[3] = {color[0], color[1], color[2]}; //HACK: I imagine that there must be a better way to do this.
	this->img->draw_point(x,y, _color);
}

int Maze::GetStartX(){ return this->m_Sx;}

int Maze::GetStartY(){ return this->m_Sy;}

int Maze::GetEndX(){ return this->m_Ex;}

int Maze::GetEndY(){ return this->m_Ey;}

void Maze::RunBreadth(bool display, unsigned int scalar, bool saveResult)
{
	std::cout << "Running Breadth!" << std::endl;
	std::queue<std::array<unsigned int, 2>> queue;
	std::map<std::array<unsigned int, 2>, std::array<unsigned int, 2>> came_from;

	queue.push({this->m_Sx, this->m_Sy});
	came_from[{this->m_Sx, this->m_Sy}] = {this->m_Sx, this->m_Sy};

	if(display)
	{
		if(scalar == 0) 
			scalar = 1;
		CImgDisplay main_disp((*this->img),"Breadth First Search", 0);

		std::array<unsigned int, 2> current = {this->m_Ex, this->m_Ey};
		std::array<unsigned int, 2> start = {this->m_Sx, this->m_Sy};

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
		std::array<unsigned int, 2> current = {this->m_Ex, this->m_Ey};
		std::array<unsigned int, 2> start = {this->m_Sx, this->m_Sy};

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

void Maze::BreadthStep(std::queue<std::array<unsigned int, 2>>& queue, 
	                   	std::map< std::array<unsigned int, 2>, std::array<unsigned int, 2> >& came_from)
{
	std::array<unsigned char, 3> grey = {126, 126, 126};
	std::array<unsigned char, 3> black = {0, 0, 0};
	std::array<unsigned char, 3> path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do BreadthStep on an empty queue." );

	std::array<unsigned int, 2> current = queue.front();
	queue.pop();

	std::vector<std::array<unsigned int,2>> neighbors = this->GetNeighbors(current);

	for(std::array<unsigned int, 2> neighbor : neighbors)
	{

		std::cout << "Checking: {" << neighbor[0] << "," << neighbor[1] << "}" << std::endl;

		if(neighbor[0] == this->m_Ex && neighbor[1] == this->m_Ey)
		{
			//Break the loop
			std::cout << "Found end!" << std::endl;
			queue = std::queue<std::array<unsigned int, 2>>();
			came_from.try_emplace(neighbor, current);
			return;
		}
		else
		{
			this->ColorPixel(neighbor, grey);
			came_from.try_emplace(neighbor, current);
			queue.push(neighbor);
		}
	}
}

void Maze::RunDijkstra(bool display, bool saveResult)
{
	std::cout << "Running Djikstra!" << std::endl;
	std::vector<std::array<unsigned int,3>> queue;
	queue.reserve(50);

	std::map<std::array<unsigned int, 2>, std::array<unsigned int, 2>> came_from;
	std::map<std::array<unsigned int, 2>, unsigned int> cost_so_far;

	came_from[{this->m_Sx, this->m_Sy}] = {this->m_Sx, this->m_Sy};
	cost_so_far[{this->m_Sx, this->m_Sy}] = 0;

	queue.emplace_back((std::array<unsigned int, 3>){this->m_Sx, this->m_Sy, 0});

	std::array<unsigned int, 2> current = {this->m_Ex, this->m_Ey};
	std::array<unsigned int, 2> start = {this->m_Sx, this->m_Sy};

	if(display)
	{
		CImgDisplay main_disp((*this->img),"Djikstra Search", 0);

	    while (!main_disp.is_closed()) 
	    {
	    	main_disp.resize(500,500).display((*this->img));
			if(!queue.empty())
			{
				for(unsigned int i = 0; i < 1; i++)
				{
					this->DijkstraStep(queue, came_from, cost_so_far);
					if(queue.empty())
						break;
				}
			}
			else if(queue.empty() && current != start)
			{

				unsigned int cond = 1; //(1 * scalar/100);
				/*if (cond == 0)
					cond = 1;*/
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

void Maze::DijkstraStep(std::vector<std::array<unsigned int, 3>>& queue, 
	                   		std::map< std::array<unsigned int, 2>, std::array<unsigned int, 2> >& came_from,
	                   		std::map< std::array<unsigned int, 2>, unsigned int>& cost_so_far)
{
	std::array<unsigned char, 3> grey = {126, 126, 126};
	std::array<unsigned char, 3> path = {255, 0, 0};
	std::array<unsigned char, 3> black = {0, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do DijkstraStep on an empty queue." );

	std::sort(queue.begin(), queue.end(), [this](auto l, auto r) {return (l[2] > r[2]); });

	std::array<unsigned int, 3> coords = queue.back();
	std::array<unsigned int, 2> current = {coords[0], coords[1]};

	queue.pop_back();

	std::vector<std::array<unsigned int,2>> neighbors = this->GetNeighbors(current);

	for(std::array<unsigned int, 2> neighbor : neighbors)
	{
		//Since the cost between is always 1, we add 1 to the new weight.
		//new_weight = cost_so_far[coords] + cost_between(coords, next)
		unsigned int new_weight = cost_so_far[current] + 1;

		std::cout << "Checking: {" << neighbor[0] << "," << neighbor[1] << "}" << std::endl;

		if(neighbor[0] == this->m_Ex && neighbor[1] == this->m_Ey)
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
				std::array<unsigned int, 3> neigh_weighted = {neighbor[0], neighbor[1], new_weight};
				queue.emplace_back(neigh_weighted);
				came_from[neighbor] = current;
			}

		}
	}
}

void Maze::RunAStar(bool display, bool saveResult)
{
	if(display)
	{
		//TODO
	}
	else
	{
		//TODO
	}
}

void Maze::AStarStep()
{
	//TODO
}

void Maze::Test()
{

}

std::vector<std::array<unsigned int,2>> Maze::GetNeighbors(std::array<unsigned int, 2> coords)
{
	return this->GetNeighbors(coords[0], coords[1]);
}

std::vector<std::array<unsigned int,2>> Maze::GetNeighbors(unsigned int x, unsigned int y)
{
	std::vector<std::array<unsigned int,2>> result;
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
			std::array<unsigned int, 2> neighbor = {x + _x, y + _y};

			if(this->IsWalkable(neighbor))
			{
				result.push_back(neighbor);
			}
			else if(neighbor[0] == this->m_Ex && neighbor[1] == this->m_Ey)
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
