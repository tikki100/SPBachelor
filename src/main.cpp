#include <iostream>
#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include "classMaze.h"



int main(int argc, const char** argv)
{
    std::cout << "P3 Hello, world!\n" << std::endl;
    CImg<unsigned char> * img  = new CImg<unsigned char>("../../examples/test01.png");

    Eng::Maze Maze(img);

    Maze.Display();

    return 0;
}
