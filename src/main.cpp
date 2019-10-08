#include <iostream>
#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include "classMaze.h"



int main(int argc, const char** argv)
{
    std::cout << "P3 Hello, world!\n" << std::endl;
    CImg<unsigned char> * img  = new CImg<unsigned char>("../../examples/test03.png");

    Eng::Maze Maze(img);

    //Maze.RunBreadth();
    Maze.RunBreadth(true, 5000);

    printf("End \n");

    return 0;
}
