#include <iostream>
#define cimg_use_png 1
#include "CImg.h"
using namespace cimg_library;

#include "classTimer.h"

#include "classMaze.h"

int main(int argc, const char** argv)
{
    bool runTests = true;

    if(runTests)
    {
        int i = 0;
        std::array<std::string, 24> tests = { "test1_100x100","test1_200x200","test1_300x300","test1_400x400","test1_500x500",
                                            "test2_100x100","test2_200x200","test2_300x300","test2_400x400",
                                            "test3_100x100","test3_200x200","test3_300x300","test3_400x400","test3_500x500",
                                            "test4_100x100","test4_200x200","test4_300x300","test4_400x400","test4_500x500",
                                            "test5_100x100","test5_200x200","test5_300x300","test5_400x400","test5_500x500",};
        for(std::string fileName : tests)
        {
            i++;
            std::cout << "\n --------------------------------" << std::endl;
            std::string path = "../../tests/" + fileName + ".png";
            std::cout << "Loading test from path " << path << std::endl;

            CImg<unsigned char> * img  = new CImg<unsigned char>(path.c_str());
            Eng::Maze Maze(img, fileName);

            Maze.RunningTests();

            std::cout << "Running breadth" << std::endl;
            {
                Eng::Timer t(fileName);
                Maze.RunBreadth();
            }
            * img = CImg<unsigned char>(path.c_str());

            std::cout << "\nRunning Dijkstra" << std::endl;
            {
                Eng::Timer t(fileName);
                Maze.RunDijkstra();
            }
            * img = CImg<unsigned char>(path.c_str());

            std::cout << "\nRunning A*" << std::endl;
            {
                Eng::Timer t(fileName);
                Maze.RunAStar();
            }
            * img = CImg<unsigned char>(path.c_str());

            std::cout << "\nRunning HPA*" << std::endl;
            {
                Eng::Timer t(fileName);
                Maze.RunHPAStar(10);
            }
            * img = CImg<unsigned char>(path.c_str());

            std::cout << "\nRunning JPS" << std::endl;
            {
                Eng::Timer t(fileName);
                Maze.RunJPS();
            }
        }
    }

    else
    {


        for(int i = 2; i <= 26; i++)
        {
            std::string fileName = "test" + std::to_string(i); 
            std::string path = "../../examples/" + fileName + ".png";
            std::cout << "Loading test from path " << path << std::endl;

            CImg<unsigned char> * img  = new CImg<unsigned char>(path.c_str());
            Eng::Maze Maze(img, fileName);

            Maze.RunJPS(false, 100, true);
            * img = CImg<unsigned char>(path.c_str());
            Maze.RunDijkstra();
            * img = CImg<unsigned char>(path.c_str());
            Maze.RunAStar();
        }
    }

    printf("End of program \n");

    return 0;

}
