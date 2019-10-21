#include <iostream>
#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include "classMaze.h"



int main(int argc, const char** argv)
{
    std::cout << "P3 Hello, world!\n" << std::endl;

    std::string filename = "test05";

    std::string path = "../../examples/" + filename + ".png";
    CImg<unsigned char> * img  = new CImg<unsigned char>(path.c_str());

    Eng::Maze Maze(img, filename);

    //Maze.RunDijkstra(true);
    //Maze.RunBreadth();
    Maze.RunBreadth(true);


    //Maze.RunBreadth(true, 500);

    printf("End of program \n");

    return 0;
}
