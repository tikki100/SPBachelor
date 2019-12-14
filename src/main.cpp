#include <iostream>
#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include "classTimer.h"

#include "classMaze.h"

int main(int argc, const char** argv)
{
	//TYPE THE EXAMPLE NAME HERE
    std::string filename = "test14";

    std::string path = "../../examples/" + filename + ".png";
    std::cout << "Loading example from path " << path << std::endl;
    CImg<unsigned char> * img  = new CImg<unsigned char>(path.c_str());

    Eng::Maze Maze(img, filename);

    //Maze.RunHPAStar(10);

    //Maze.Test();

    //Maze.RunAStar();

    //Maze.RunDijkstra();

    //Maze.RunHPAStar(10);

    Maze.RunJPS();

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
    * img = CImg<unsigned char>(path.c_str());*/
    /*{
        Eng::Timer t;
        Maze.RunHPAStar(100);
    }*/
   

    /*
    for(int i = 1; i < 10; i++)
    {
    	std::string filename = "test0";
    	filename += std::to_string(i);
    	std::string path = "../../examples/" + filename + ".png";
	    std::cout << "Loading example from path " << path << std::endl;
	    CImg<unsigned char> * img  = new CImg<unsigned char>(path.c_str());

	    Eng::Maze Maze(img, filename);
	    //Maze.RunBreadth(true, 40000);
	    Maze.RunBreadth();
	    * img = CImg<unsigned char>(path.c_str());
	    Maze.RunDijkstra();
	    * img = CImg<unsigned char>(path.c_str());
	    Maze.RunAStar();
	}*/





    //Maze.RunBreadth(true, 500);

    printf("End of program \n");

    return 0;

}
