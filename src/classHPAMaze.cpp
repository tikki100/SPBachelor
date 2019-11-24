#include "classHPAMaze.h"

namespace Eng
{ //Start of Eng

	HPAMaze::HPAMaze(CImg<unsigned char> * imgFile, unsigned int maxLevel, unsigned int clusterSize)
	{
		this->depth = maxLevel;
		this->img = imgFile;

		this->Clusters.reserve(maxLevel);
		unsigned int width, height;

		for (int i = 0; i < maxLevel; i++)
		{
			std::cout << "Creating clusters for level " << i << std::endl;
			//For each level in the map, the cluster size grows larger,
			//since our abstraction map grows larger. We therefore increase
			//the clustersize by a randomly chosen integer - in this chase,
			//we multiply by 3, for each level.
			if(i != 0)
				clusterSize = clusterSize * 3;

			width = std::ceilf(imgFile->width() / clusterSize);
			height = std::ceilf(imgFile->height() / clusterSize);

			if (width <= 1 && height <= 1)
			{
				depth = i;
				break;
			}
			this->Clusters.emplace_back(this->BuildClusters(i, clusterSize, width, height));
		}

	}

	HPAMaze::~HPAMaze()
	{

	}

	std::vector<Cluster> HPAMaze::BuildClusters(unsigned int level, unsigned int clusterSize, unsigned int width, unsigned int height)
	{
		std::vector<Cluster> resClusters;
		resClusters.reserve(height*width);
		unsigned int imgWidth = this->img->width();
		unsigned int imgHeight = this->img->height();

		for(unsigned int i = 0; i < height; i++)
		{
			for(unsigned int j = 0; j < width; j++)
			{
				Cluster cluster;

				Pixel Min = { j * clusterSize, i * clusterSize };
				Pixel Max = {std::min(Min.x + clusterSize - 1, (imgWidth - 1)),
							std::min(Min.y + clusterSize - 1, (imgHeight - 1))};

				cluster.SetMin(Min);
				cluster.SetMax(Max);

				resClusters.emplace_back(cluster);
			}
		}
		return resClusters;

	}
} //End of namespace Eng
