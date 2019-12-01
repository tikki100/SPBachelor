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

		std::cout << "Build a total of " << resClusters.size() << " clusters." << std::endl;

		for(Cluster cluster : resClusters)
		{
			this->CreateIntraEdges(cluster);
			std::cout << "Cluster: " << cluster << std::endl;
			std::cout << "Cluster has " << cluster.trans.size() << " transition pixels." << std::endl;
			for (const auto& p : cluster.trans ) 
			{
		        for(Edge ed : p.second)
		        {
		        	std::cout << ed.type << " Start: " << ed.s << " End: "<< ed.e << std::endl;
		        }
			}
		}


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

	void HPAMaze::CreateIntraEdges(Cluster& c)
	{
		//std::cout << "Called CreateIntraEdges on " << c << std::endl;
		for (const auto& p1 : c.trans ) 
		{
			for (const auto& p2 : c.trans ) 
			{
				//std::cout << p1.first << " & " << p2.first << std::endl;
				if(p1.first == p2.first)
				{
					continue;
				}
				auto [path, weight] = this->GetPath(c, p1.first, p2.first);
				//std::cout << "p.size = " << path.size() << std::endl;
	        	if(path.size() > 0)
	        	{
					Edge newedge;
					newedge.Set(p1.first, p2.first, weight, Edge::INTRA);
					c.trans[p1.first].emplace_back(newedge);
	        	}
			}
		}
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

	std::tuple<std::map<Pixel, Pixel>, float> HPAMaze::GetPath(Cluster& c, Pixel start, Pixel end)
	{
		std::priority_queue<WeightedPixel> queue;

		std::map< Pixel, Pixel> came_from;
		std::map< Pixel, float> cost_so_far;

		came_from[start] = start;
		cost_so_far[start] = 0;

		bool FoundEnd = false;

		queue.emplace((WeightedPixel){start.x, start.y, 0});

		//std::cout << "Pathfinding between " << start << " " << end << std::endl;

		while(!queue.empty())
		{
			WeightedPixel coords = queue.top();
			Pixel current = {coords.x, coords.y};
			//std::cout << "Checking " << current << std::endl;
			queue.pop();
			std::vector<Pixel> neighbors = this->GetPixelNeighbors(current, c);
			for(Pixel neighbor : neighbors)
			{
				float cost = 0.0f;
				//The cost to move in a straight line is 1, while diagonally is sqrt(2)
				if((current.x - neighbor.x == 0) || (current.y - neighbor.y == 0))
					cost = 1.0f;
				else
					cost = std::sqrt(2);
				
				float new_weight = cost_so_far[current] + cost;

				if(neighbor == end)
				{
					//Break the loop
					came_from[neighbor] = current;
					cost_so_far[neighbor] = new_weight;
					FoundEnd = true;
					queue = std::priority_queue<WeightedPixel>();
					break;
				}
				else
				{
					//this->ColorPixel(neighbor, grey);
					if(cost_so_far.count(neighbor) == 0 || new_weight < cost_so_far[neighbor])
					{
						cost_so_far[neighbor] = new_weight;
						std::cout << current << neighbor << " w: " << new_weight << " h:" << this->GetHeuristicCost(end, neighbor) << std::endl;
						float priority = new_weight + this->GetHeuristicCost(end, neighbor);
						WeightedPixel neigh_weighted = {neighbor.x, neighbor.y, priority};
						queue.emplace(neigh_weighted);
						came_from[neighbor] = current;
					}

				}
			}
		}

		std::map< Pixel, Pixel> path;
		float resWeight = 0.0f;
		if(FoundEnd)
		{
			resWeight = cost_so_far[end];
			while(end != start)
			{
				path[end] = came_from[end];
				end = came_from[end];
			}
		}
		/*else
			std::cout << "Found no path" << std::endl;*/
		return std::tuple(path, resWeight);

	}

	float HPAMaze::GetHeuristicCost(Pixel goal, Pixel current)
	{
		unsigned long long res = 0;
		//HACK - We are open to an overflow error.
		//We always want a positive integer, since we use unsigned ints.
		if(goal.x >= current.x)
		{
			  res += std::pow(goal.x - current.x, 2);
		}
		else
		{
			res += std::pow(current.x - goal.x, 2);
		}

		if(goal.y >= current.y)
		{
			res += std::pow(goal.y - current.y,2);
		}
		else
		{
			res += std::pow(current.y - goal.y,2);
		}

		//Euclidean distance
		return std::sqrt(res);
	}

	std::vector<Pixel> HPAMaze::GetPixelNeighbors(Pixel p, Cluster& c)
	{
		std::vector<Pixel> result;
		result.reserve(8);
	
		for(int _x = -1; _x < 2; _x++) //Check all 8 neighboring pixels.
		{
			if(p.x == 0 && _x == -1) //If we move too far left, skip
				continue;
			else if(p.x == (this->img->width()-1) && _x == 1) //If we're too  far right, skip
				continue;
			for(int _y = -1; _y < 2; _y++)
			{
				if(p.y == 0 && _y == -1)  //If we're above the picture, skip
					continue;
				else if(p.y == (this->img->height()-1) && _y == 1) //If we're below the picture, skip
					continue;
				else if(p.x == p.x + _x && p.y == p.y + _y) //If we're on the current pixel, skip
					continue;
				Pixel neighbor = {p.x + _x, p.y + _y};
	
				if(this->IsWalkable(neighbor) && c.Contains(neighbor))
				{
					result.push_back(neighbor);
				}
			}
		}

		return result;
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

