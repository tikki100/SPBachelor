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

		for(unsigned int i = 0; i < height; i++) //Generate each cluster
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

		//Detect adjacent clusters and build entrances (INTER edges)
		for(unsigned int i = 0; i < resClusters.size(); i++)
		{
			for(unsigned int j = i+1; j < resClusters.size(); j++)
			{
				LocCluster loc = this->IsClusterAdjacent(resClusters[i], resClusters[j]);
				if(loc != LocCluster::NONE)
				{
					if(level == 0)
					{
						CreateFirstBorderEntrances(resClusters[i], resClusters[j], loc);
					}
					else
					{
						CreateAbstractBorderEntrances(resClusters[i], resClusters[j], loc);
					}
				}
			}
		}

		//Build paths inside the clusters (INTRA edges)


		return resClusters;

	}

	HPAMaze::LocCluster HPAMaze::IsClusterAdjacent(Cluster& c1, Cluster& c2)
	{
		LocCluster res = LocCluster::NONE;

		if(c1.GetMin().x == c2.GetMin().x)
		{
			if( (c1.GetMax().y + 1) == c2.GetMin().y)
			{
				res = LocCluster::BELOW;
			}
			else if((c2.GetMax().y + 1) == c1.GetMin().y)
				res = LocCluster::ABOVE;
		}
		else if(c1.GetMin().y == c2.GetMin().y)
		{
			if( (c1.GetMax().x + 1) == c2.GetMin().x)
			{
				res = LocCluster::RIGHT;
			}
			else if((c2.GetMax().x + 1) == c1.GetMin().x)
			{
				res = LocCluster::LEFT;
			}
		}

		return res;
	}

	void HPAMaze::CreateFirstBorderEntrances(Cluster& c1, Cluster& c2, HPAMaze::LocCluster loc)
	{
		unsigned int lineSize = 0;
		if(loc == LocCluster::NONE)
			throw std::invalid_argument( "Got loc none in CreateFirstBorderEntrances" );
		else if(loc == LocCluster::RIGHT)
		{
			for(unsigned int i = c1.GetMin().y; i < c1.GetMax().y + 1; i++)
			{
				Pixel p1 = {c1.GetMax().x , i};
				Pixel p2 = {c2.GetMin().x , i};

				if(this->IsWalkable(p1) && this->IsWalkable(p2))
					lineSize++;
				else
				{
					this->CreateFirstInterEdges(c1, c2, lineSize, i, true);
				}
			}
			//If lineSize is bigger than one, we need to create the last entrance as well.
			this->CreateFirstInterEdges(c1, c2, lineSize, c1.GetMax().y+1, true);

		}
		else if(loc == LocCluster::LEFT)
		{
			this->CreateFirstBorderEntrances(c2, c1, LocCluster::RIGHT);
		}
		else if(loc == LocCluster::ABOVE)
			this->CreateFirstBorderEntrances(c2, c1, LocCluster::BELOW);
		
		else if(loc == LocCluster::BELOW)
		{
			for(unsigned int i = c1.GetMin().x; i < c1.GetMax().x + 1; i++)
			{
				Pixel p1 = {i , c1.GetMax().y};
				Pixel p2 = {i , c2.GetMin().y};

				if(this->IsWalkable(p1) && this->IsWalkable(p2))
					lineSize++;
				else
				{
					this->CreateFirstInterEdges(c1, c2, lineSize, i, false);
				}

			}
			//If lineSize is bigger than one, we need to create the last entrance as well.
			this->CreateFirstInterEdges(c1, c2, lineSize, c1.GetMax().x+1, false);

		}
		
	}

	void HPAMaze::CreateFirstInterEdges(Cluster& c1, Cluster& c2, unsigned int& lineSize, unsigned int i, bool x)
	{
		if(lineSize > 0)
		{
			if(lineSize <= 5) //The entrance is too small, so we only add 1 transition
			{
				unsigned int middle = i - std::floorf(lineSize/2);
				Pixel c1trans, c2trans;
				if(x)
				{
					c1trans = {c1.GetMax().x, middle};
					c2trans = {c2.GetMin().x, middle};
				}
				else
				{
					c1trans = {middle, c1.GetMax().y};
					c2trans = {middle, c2.GetMin().y};
				}

				Edge c1edge;
				c1edge.Set(c1trans, c2trans, 1.0f, Edge::INTER);
				c1.trans[c1trans].emplace_back(c1edge);

				Edge c2edge;
				c2edge.Set(c2trans, c1trans, 1.0f, Edge::INTER);
				c2.trans[c2trans].emplace_back(c2edge);

			}
			else //We add two transition points.
			{
				Pixel c1trans, c2trans, c1transend, c2transend;
				if(x)
				{
					c1trans = {c1.GetMax().x, i - lineSize};
					c2trans = {c2.GetMin().x, i - lineSize};
					if(i == 0)
						throw "i == 0 in CreateFirstBorderEntrances!";
					c1transend = {c1.GetMax().x, i - 1 };
					c2transend = {c2.GetMin().x, i - 1 };
				}
				else
				{
					c1trans = {i - lineSize, c1.GetMax().y};
					c2trans = {i - lineSize, c2.GetMin().y};
					if(i == 0)
						throw "i == 0 in CreateFirstBorderEntrances!";
					c1transend = {i - 1, c1.GetMax().y};
					c2transend = {i - 1, c2.GetMin().y};
				}

				Edge c1edge;
				c1edge.Set(c1trans, c2trans, 1.0f, Edge::INTER);
				c1.trans[c1trans].emplace_back(c1edge);

				Edge c1edgeend;
				c1edgeend.Set(c1transend, c2transend, 1.0f, Edge::INTER);
				c1.trans[c1transend].emplace_back(c1edgeend);

				Edge c2edge;
				c2edge.Set(c2trans, c1trans, 1.0f, Edge::INTER);
				c2.trans[c2trans].emplace_back(c2edge);

				Edge c2edgeend;
				c2edgeend.Set(c2transend, c1transend, 1.0f, Edge::INTER);
				c2.trans[c2transend].emplace_back(c2edgeend);

			}
			lineSize = 0;
		}
	}

	void HPAMaze::CreateAbstractBorderEntrances(Cluster& c1, Cluster& c2, LocCluster loc)
	{
		//TODO
	}

	bool HPAMaze::IsWalkable(Pixel p)
	{
		RGB walkable = {255, 255, 255};
		RGB goal = {0, 0, 255};
		RGB start = {0, 255, 0};
		if((this->GetColor(p) == walkable) || (this->GetColor(p) == goal) || (this->GetColor(p) == start))
		{
			return true;
		}
		return false;
	}

	void HPAMaze::Test()
	{
		std::cout << "Test!" << std::endl;
	}

	const RGB HPAMaze::GetColor(Pixel p)
	{
		return {(*this->img)(p.x,p.y, 0), (*this->img)(p.x,p.y, 1), (*this->img)(p.x,p.y, 2)};
	}

} //End of namespace Eng

