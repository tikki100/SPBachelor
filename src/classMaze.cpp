#include "classMaze.h"

namespace Eng
{ //Start of Eng

Maze::Maze(CImg<unsigned char> * imgFile)
{
	this->img = imgFile;

	this->_Sx = 0;
	this->_Sy = 0;

	this->_Ex = 0;
	this->_Ey = 0;

	this->_startFound = false;
	this->_endFound = false;

	this->Verify();
}

Maze::~Maze()
{

}

void Maze::Verify()
{
	if(this->img->spectrum() != 3)
	{
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
    	//std::cout << "x:" << x << " y:" << y << " R:"<< (int)(*this->img)(x,y, 0) << "G:" << (int)(*this->img)(x,y, 1) << "B" << (int)(*this->img)(x,y, 2) << std::endl;
    	
        if(this->IsColor(x,y,0,255,0)){
        	if(this->_startFound == false){
				this->_Sx = x;
	        	this->_Sy = y;
	        	this->_startFound = true;
	        	std::cout << "Found starting pixel at " << x << "," << y << std::endl;
        	}
        	else
        	{
        		throw std::invalid_argument( "Image contained more than one starting point" );
        	}
        }
        else if(this->IsColor(x,y,0,0,255)){
        	if(this->_endFound == false){
	        	this->_Ex = x;
	        	this->_Ey = y;
	        	this->_endFound = true;
	        	std::cout << "Found end pixel at " << x << "," << y << std::endl;
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
    return (_startFound && _endFound);
}

void Maze::Display()
{
	CImg<unsigned char> * result = this->img;
	if(this->img->width() <= 100 && this->img->height() <= 100)
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
	}


	CImgDisplay main_disp(*(result),"Test image 2", 0);

    while (!main_disp.is_closed()) 
    {
        main_disp.wait();
    }
}

bool Maze::IsWalkable(unsigned int x, unsigned int y)
{
	if(this->IsColor(x, y, 255, 255, 255) 
		|| (this->_Sx == x && this->_Sy == y)
		|| (this->_Ex == x && this->_Ey == y))
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

std::array<unsigned char, 3> Maze::GetColor(unsigned int x, unsigned int y)
{
	return {(*this->img)(x,y, 0), (*this->img)(x,y, 1), (*this->img)(x,y, 2)};
}

int Maze::GetStartX(){ return this->_Sx;}

int Maze::GetStartY(){ return this->_Sy;}

int Maze::GetEndX(){ return this->_Ex;}

int Maze::GetEndY(){ return this->_Ey;}

void Maze::RunBreadth()
{

}

}//End of namespace Eng
