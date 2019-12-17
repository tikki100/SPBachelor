#include <iostream>
#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include "classTimer.h"

#include "classMaze.h"

int main(int argc, const char** argv)
{
	//TYPE THE EXAMPLE NAME HERE
    std::string filename = "test13";

    std::string path = "../../examples/" + filename + ".png";
    std::cout << "Loading example from path " << path << std::endl;
    CImg<unsigned char> * img  = new CImg<unsigned char>(path.c_str());

    Eng::Maze Maze(img, filename);

    //Maze.RunHPAStar(10);

    //Maze.Test();

    //Maze.RunAStar();

    //Maze.RunDijkstra();

    //Maze.RunHPAStar(10);

    //Maze.RunJPS(true, 1, true);

    //Maze.RunBreadth(true, 40000);
    /*{
        Eng::Timer t;
        Maze.RunBreadth();
    }
    * img = CImg<unsigned char>(path.c_str());
    {
        Eng::Timer t;
        Maze.RunDijkstra();
    }
    * img = CImg<unsigned char>(path.c_str());
    {
        Eng::Timer t;
        Maze.RunAStar();
    }
    * img = CImg<unsigned char>(path.c_str());
    {
        Eng::Timer t;
        Maze.RunJPS();
    }*/
    /** img = CImg<unsigned char>(path.c_str());
    {
        Eng::Timer t;
        Maze.RunHPAStar(100);
    }*/
   

    
    for(int i = 2; i < 22; i++)
    {
    	std::string filename = "test";
    	filename += std::to_string(i);
    	std::string path = "../../examples/" + filename + ".png";
	    std::cout << "Loading example from path " << path << std::endl;
	    CImg<unsigned char> * img  = new CImg<unsigned char>(path.c_str());

	    Eng::Maze Maze(img, filename);
        {
            Eng::Timer t;
            Maze.RunBreadth();
        }
        * img = CImg<unsigned char>(path.c_str());
        {
            Eng::Timer t;
            Maze.RunDijkstra();
        }
        * img = CImg<unsigned char>(path.c_str());
        {
            Eng::Timer t;
            Maze.RunAStar();
        }
        * img = CImg<unsigned char>(path.c_str());
        {
            Eng::Timer t;
            Maze.RunJPS();
        }
	}





    //Maze.RunBreadth(true, 500);

    printf("End of program \n");

    return 0;

}
