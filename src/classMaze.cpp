#include "classMaze.h"

namespace Eng
{ //Start of Eng

Maze::Maze(CImg<unsigned char> * imgFile)
{
	this->img = imgFile;
	this->imgBackup = imgFile;

	this->m_Sx = 0;
	this->m_Sy = 0;

	this->m_Ex = 0;
	this->m_Ey = 0;

	this->m_startFound = false;
	this->m_endFound = false;

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
    	/*
    	//Prints a specific pixels color
    	if(x == 455 && y == 454)
    		std::cout << "x:" << x << " y:" << y << " R:"<< (int)(*this->img)(x,y, 0) << "G:" << (int)(*this->img)(x,y, 1) << "B" << (int)(*this->img)(x,y, 2) << std::endl;
    	*/
        if(this->IsColor(x,y,0,255,0)){
        	if(this->m_startFound == false){
				this->m_Sx = x;
	        	this->m_Sy = y;
	        	this->m_startFound = true;
	        	std::cout << "Found starting pixel at (" << x << "," << y << ")" << std::endl;
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
	        	std::cout << "Found end pixel at (" << x << "," << y << ")" << std::endl;
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

bool Maze::IsWalkable(std::array<unsigned int, 2> coords)
{
	return this->IsWalkable(coords[0], coords[1]);
}

bool Maze::IsWalkable(unsigned int x, unsigned int y)
{
	if(this->IsColor(x, y, 255, 255, 255) 
		|| (this->m_Sx == x && this->m_Sy == y)
		|| (this->m_Ex == x && this->m_Ey == y))
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

void Maze::RunBreadth(bool display, unsigned int scalar)
{
	this->img = this->imgBackup;
	//CImgDisplay main_disp((*this->img),"Breadth First Search", 0);
	std::queue<std::array<unsigned int, 2>> queue;
	queue.push({m_Sx, m_Sy});
	std::map<std::array<unsigned int, 2>, std::array<unsigned int, 2>> came_from;
	came_from[{m_Sx, m_Sy}] = {m_Sx, m_Sy};

	if(display)
	{
		if(scalar == 0) 
			scalar = 1;
		CImgDisplay main_disp((*this->img),"Breadth First Search", 0);
		std::array<unsigned int, 2> current = {this->m_Ex, this->m_Ey};
		std::array<unsigned int, 2> start = {m_Sx, m_Sy};
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
				for(unsigned int i = 0; i < 1 * scalar/100; i++)
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
	    	//main_disp.wait(10);
	    }
	}

	else
	{
		while(!queue.empty())
		{
			this->BreadthStep(queue, came_from);
		}
		std::array<unsigned int, 2> current = {this->m_Ex, this->m_Ey};
		std::array<unsigned int, 2> start = {m_Sx, m_Sy};
		while(current != start)
		{
			this->ColorPixel(current, {255, 0, 0});
			current = came_from[current];
		}
		this->ColorPixel(start, {255, 0, 0});
		this->img->save("../../examples/breadth.png");
	}


}

void Maze::BreadthStep(std::queue<std::array<unsigned int, 2>>& queue, 
	                   std::map<std::array<unsigned int, 2>, std::array<unsigned int, 2>>& came_from)
{
	std::array<unsigned char, 3> grey = {126, 126, 126};
	std::array<unsigned char, 3> path = {255, 0, 0};

	std::array<unsigned int, 2> coords = queue.front();
	queue.pop();

	if(coords[0] == m_Ex && coords[1] == m_Ey)
	{
		//Break the loop
		std::cout << "Found end!" << std::endl;
		queue = std::queue<std::array<unsigned int, 2>>();
		return;
	}

	for(int _x = -1; _x < 2; _x++)
	{
		if(coords[0] == 0 && _x == -1)
			continue;
		else if(coords[0] == (this->img->width()-1) && _x == 1)
			continue;
		for(int _y = -1; _y < 2; _y++)
		{
			if(coords[1] == 0 && _y == -1)
				continue;
			else if(coords[1] == (this->img->height()-1) && _y == 1)
				continue;
			std::array<unsigned int, 2> current = {coords[0] + _x, coords[1] + _y};
			if(this->IsWalkable(current))
			{
				this->ColorPixel(current, grey);
				came_from.try_emplace(current, coords);
				queue.push(current);
			}
		}
	}
}



void Maze::Test()
{
	unsigned int test = 0;
	for(int _x = -1; _x < 2; _x++)
	{
		std::cout << test + _x << std::endl;
	}
}

}//End of namespace Eng
