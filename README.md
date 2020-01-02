# The Shortest Path Problem In Maze C++ Code

This project is the result of a bachelor thesis written on finding the shortest path on a grid.  The project is written in C++ and includes the following algorithms:

* Breadth First Search
* Dijkstra's Algorithm
* A* Shortest Path
* HPA* Shortest Path
* Jump point Search


## Getting Started

These instructions will tell you how to compile the project and prepare it, such that you may run it.

### Prerequisites
Before continuing, you will need the following:

* [CMake](https://cmake.org/download/) - Version 3.15.3 or above
* GCC - Version 9.2.0 or above. Use HomeBrew for Mac or use Visual Studio for Windows
* [ImageMagick](https://imagemagick.org/script/download.php) - Version 7.0.8-66 or above
* [CImg](http://cimg.eu/download.shtml) - Version 2.7.2 or above (Included as a submodule in the vendor folder)

Furthermore, for Windows users, you will need:

* [Visual Studio Community Edition 2019](https://visualstudio.microsoft.com/downloads/) - With the C++ submodule

And for Mac users you will need:

* [XCode](https://apps.apple.com/dk/app/xcode/id497799835?l=da&mt=12) - Version 11.0 or above
* [XQuartz](https://www.xquartz.org/) - Version 2.7.11 or above 

### Installing

Either clone or download the repository into an empty folder.

```git
git clone https://github.com/tikki100/SPBachelor.git
```

Next, we need to initliaze the submodule. Navigate to the folder with the project with a git terminal, and enter the following commands:

```
git submodule init
git submodule update
```

Lastly, in the terminal, generate the project to the IDE you wish to use. Fx

```
cmake -G "XCode" -B build/src
```


## Usage

The project starts in the file `main.cpp` and is set up to run tests by default. An image is loaded into a CImg object, and the engines Maze is initiated. Please note, that a png picture only may have 3 spectrums saved to it.

```C++
CImg<unsigned char> * img  = new CImg<unsigned char>("path/to/picture.png");
Eng::Maze Maze(img, "picture");
```

From here, you can run the different algorithms by typing them out as such:

```C++
Maze.RunBreadth();
Maze.RunDijkstra();
Maze.RunAStar();
Maze.RunHPAStar(clusterSize);
Maze.RunJPS();
```

Please note, that once a test has been run, it colours the path. Therefore, you will have to reinstansiate the picture before running a new algorithm. This can be done by doing:

```C++
* img = CImg<unsigned char>("path/to/picture.png");
```


## Authors

* **Oliver Madsen** - [tikki100](https://github.com/tikki100)
