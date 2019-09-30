#include <iostream>
#define cimg_use_jpeg 1
#include "CImg.h"
using namespace cimg_library;

#include "classMaze.h"



int main(int argc, const char** argv)
{
    std::cout << "P3 Hello, world!\n" << std::endl;
    CImg<unsigned char> * img  = new CImg<unsigned char>("../../examples/test03.png");

    Eng::Maze Maze(img);

    std::cout << "Testing 0, 0: " << Maze.IsWalkable(0, 0) << std::endl;

    Maze.Display();

    return 0;
}
