#include "classMaze.h"

namespace Eng
{ //Start of Eng

Maze::Maze(CImg<unsigned char> * imgFile)
{
	this->img = imgFile;

	this->Sx = -1;
	this->Sy = -1;

	this->Ex = -1;
	this->Ey = -1;

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
        	if(this->Sx == -1 && this->Sy == -1){
				this->Sx = x;
	        	this->Sy = y;
	        	std::cout << "Found starting pixel at " << x << "," << y << std::endl;
        	}
        	else
        	{
        		throw std::invalid_argument( "Image contained more than one starting point" );
        	}
        }
        else if(this->IsColor(x,y,0,0,255)){
        	if(this->Ex == -1 && this->Ey == -1){
	        	this->Ex = x;
	        	this->Ey = y;
	        	std::cout << "Found end pixel at " << x << "," << y << std::endl;
	       	}
	       	else
	       	{
	       		throw std::invalid_argument( "Image contained more than one ending point" );
	       	}
        }
        //In theory, we could break out early - but doing so, wouldn't ensure against
        //multiple starting- or ending points. 
        /*if(this->Sx > -1 && this->Ex > -1){
        	std::cout << "Breaking" << std::endl;
        	x = this->img->width();
        	y = this->img->height();
        	break;
        }*/
    }
    if(this->Sx > -1
    	&& this-> Sy > -1
    	&& this-> Ex > -1
    	&& this-> Ey > -1)
    {
    	return true;
    }
    return false;
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

bool Maze::IsWalkable(int x, int y)
{
	if(this->IsColor(x, y, 255, 255, 255) 
		|| (this->Sx == x && this->Sy == y)
		|| (this->Ex == x && this->Ey == y))
	{
		return true;
	}
	return false;
}

bool Maze::IsColor(int x, int y, int red, int green, int blue)
{
	if(0 <= red && red <= 255
		&& 0 <= green && green <= 255
		&& 0 <= blue && blue <= 255)
	{
		if((*this->img)(x,y, 0) == red
	        && (*this->img)(x,y,1) == green
	        && (*this->img)(x,y,2) == blue){
			return true;
		}
	}
	return false;
}

int Maze::GetStartX(){ return this->Sx;}

int Maze::GetStartY(){ return this->Sy;}

int Maze::GetEndX(){ return this->Ex;}

int Maze::GetEndY(){ return this->Ey;}

}//End of namespace Eng
