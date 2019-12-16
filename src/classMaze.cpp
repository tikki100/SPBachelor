#include "classMaze.h"

namespace Eng
{ //Start of Eng

Maze::Maze(CImg<unsigned char> * imgFile, std::string name)
{
	//Saving variables
	this->img = imgFile;
	this->m_name = name;

	//Inital vlaues
	this->m_Sx = 0;
	this->m_Sy = 0;
	this->m_Start = {0,0};

	this->m_Ex = 0;
	this->m_Ey = 0;
	this->m_End = {0,0};

	this->m_startFound = false;
	this->m_endFound = false;

	//Initialize function
	this->Verify();
}

Maze::~Maze()
{

}

void Maze::Verify()
{
	if(this->img->spectrum() != 3)
	{
		std::cout << this->img->spectrum() << std::endl;
		throw std::invalid_argument( "Image does not contain 3 spectrums (RGB profile)" );
	}

	if((this->FindPoints()) == false)
	{
		throw std::invalid_argument( "Image does not contain a green starting point and a blue ending point" );
	}
}

bool Maze::FindPoints()
{
	cimg_forXY(*(this->img), x,y)
    {
        if(this->IsColor(x,y,0,255,0)){
        	if(this->m_startFound == false){
				this->m_Sx = x;
	        	this->m_Sy = y;
	        	this->m_Start = {m_Sx, m_Sy};
	        	this->m_startFound = true;
	        	std::cout << "Found starting pixel at {" << x << "," << y << "}" << std::endl;
        	}
        	else
        	{
        		throw std::invalid_argument( "Image contained more than one starting point" );
        	}
        }
        else if(this->IsColor(x,y,0,0,255)){
        	if(this->m_endFound == false){
	        	this->m_Ex = x;
	        	this->m_Ey = y;
	        	this->m_End = {m_Ex,m_Ey};
	        	this->m_endFound = true;
	        	std::cout << "Found end pixel at {" << x << "," << y << "}" << std::endl;
	       	}
	       	else
	       	{
	       		throw std::invalid_argument( "Image contained more than one ending point" );
	       	}
        }
        //In theory, we could break out early by maximising x and y -
        //but doing so, wouldn't ensure against multiple starting
        //or ending points. 
    }
    return (m_startFound && m_endFound);
}

bool Maze::IsWalkable(WeightedPixel coords)
{
	return this->IsWalkable(coords.x, coords.y);
}

bool Maze::IsWalkable(Pixel coords)
{
	return this->IsWalkable(coords.x, coords.y);
}

bool Maze::IsWalkable(unsigned int x, unsigned int y)
{
	RGB white = {255, 255, 255};
	RGB grey = {126, 126, 126};
	RGB purple = {255, 0, 255};

	if(this->IsColor(x, y, white) || this->IsColor(x,y, grey) || this->IsColor(x, y, purple))
	{
		return true;
	}
	return false;
}

bool Maze::IsColor(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	RGB color = {r, g, b};
	Pixel coords = {x,y};
	return this->IsColor(coords, color);
}

bool Maze::IsColor(Pixel& coords, unsigned char r, unsigned char g, unsigned char b)
{
	RGB color = {r, g, b};
	return this->IsColor(coords, color);
}

bool Maze::IsColor(Pixel& coords, RGB& color)
{
	RGB _checkColor = this->GetColor(coords);

	if(_checkColor == color){
		return true;
	}
	return false;
}

bool Maze::IsColor(unsigned int x, unsigned int y, RGB& color)
{
	Pixel coords = {x, y};
	return this->IsColor(coords, color);
}

const RGB Maze::GetColor(unsigned int x, unsigned int y)
{
	return {(*this->img)(x,y, 0), (*this->img)(x,y, 1), (*this->img)(x,y, 2)};
}

const RGB Maze::GetColor(Pixel& coords)
{
	return this->GetColor(coords.x, coords.y);
}

const RGB Maze::GetColor(WeightedPixel& coords)
{
	return this->GetColor(coords.x, coords.y);
}

void Maze::ColorPixel(WeightedPixel coords, RGB color)
{
	this->ColorPixel(coords.x, coords.y, color);
}

void Maze::ColorPixel(Pixel coords, RGB color)
{
	this->ColorPixel(coords.x, coords.y, color);
}

void Maze::ColorPixel(unsigned int x, unsigned int y, RGB color)
{
	const unsigned char _color[3] = {color.R, color.G, color.B}; //HACK: I imagine that there must be a better way to do this.
	this->img->draw_point(x,y, _color);
}

void Maze::RunBreadth(bool display, unsigned int scalar, bool saveResult)
{
	std::cout << "Running Breadth!" << std::endl;
	std::queue<Pixel> queue;
	std::unordered_map<Pixel, Pixel> came_from;


	queue.emplace(this->m_Start);
	came_from.insert_or_assign(this->m_Start, this->m_Start);

	this->m_endFound = false;

	
	if(display)
	{
		if(scalar == 0) 
			scalar = 1;
		CImgDisplay main_disp((*this->img),"Breadth First Search", 0);

		Pixel current = this->m_End;
		Pixel start = this->m_Start;

		unsigned int i;
	    while (!main_disp.is_closed()) 
	    {
	    	main_disp.resize(500,500).display((*this->img));
			if(!queue.empty())
			{
				i++;
				if(i % 100 == true)
				{
					std::cout << "Queue size: " << queue.size() << std::endl;
				}
				for(unsigned int i = 0; i < 1 * scalar; i++)
				{
					this->BreadthStep(queue, came_from);
					if(queue.empty())
						break;
				}
			}
			else if(queue.empty() && current != start)
			{
				unsigned int cond = (1 * scalar/100);
				if (cond == 0)
					cond = 1;
				for(unsigned int i = 0; i < cond; i++)
				{
					this->ColorPixel(current, {255, 0, 0});
			        current = came_from[current];
			        if(current == start)
			        {
			        	this->ColorPixel(current, {255, 0, 0});
			        	break;
			        }
				}
			}
	    }
	    if(saveResult)
	    {
	    	std::string saveName = this->exampleFolder + this->m_name + "_breadth.png";
	    	this->img->save(saveName.c_str());
	    }
	}

	else
	{
		Pixel current = this->m_End;
		Pixel start = this->m_Start;
		unsigned int i = 0;

		while(!queue.empty())
		{
			this->BreadthStep(queue, came_from);
		}
		while(current != start)
		{
			this->ColorPixel(current, {255, 0, 0});
			current = came_from[current];
		}
		this->ColorPixel(start, {255, 0, 0});

		std::string saveName = this->exampleFolder + this->m_name + "_breadth.png";

		this->img->save(saveName.c_str());
	}
	

}

void Maze::BreadthStep(std::queue<Pixel>& queue, 
	                   	std::unordered_map< Pixel, Pixel >& came_from)
{
	RGB grey = {126, 126, 126};
	RGB black = {0, 0, 0};
	RGB path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do BreadthStep on an empty queue." );

	Pixel current = queue.front();
	queue.pop();

	std::vector<Pixel> neighbors = this->GetNeighbors(current);

	for(Pixel& neighbor : neighbors)
	{
		if(neighbor == this->m_End)
		{
			//Break the loop
			std::cout << "Found end!" << std::endl;
			this->m_endFound = true;
			queue = std::queue<Pixel>();
			came_from.emplace(neighbor, current);
			return;
		}
		else
		{
			if(came_from.count(neighbor) == 0)
			{
				this->ColorPixel(neighbor, grey);
				came_from.emplace(neighbor, current);
				queue.emplace(neighbor);
			}
		}
	}
}

void Maze::RunDijkstra(bool display, unsigned int scalar, bool saveResult)
{
	std::cout << "Running Djikstra!" << std::endl;
	std::priority_queue<WeightedPixel> queue;

	std::unordered_map<Pixel, Pixel> came_from;
	std::unordered_map<Pixel, float> cost_so_far;

	came_from[this->m_Start] = this->m_Start;
	cost_so_far[this->m_Start] = 0.0f;

	queue.emplace((WeightedPixel){this->m_Sx, this->m_Sy, 0});

	Pixel current = this->m_End;
	Pixel start = this->m_Start;

	this->m_endFound = false;

	if(display)
	{
		CImgDisplay main_disp((*this->img),"Djikstra Search", 0);
		if(scalar == 0) 
			scalar = 1;

	    while (!main_disp.is_closed()) 
	    {
	    	main_disp.resize(500,500).display((*this->img));
			if(!queue.empty())
			{
				for(int i = 0; i < 1*scalar; i++)
				{
					this->DijkstraStep(queue, came_from, cost_so_far);
					if(queue.empty())
						break;
				}
			}
			else if(queue.empty() && current != start)
			{
				unsigned int cond = (1 * scalar/100);
				if (cond == 0)
					cond = 1;
				for(int i = 0; i < 1*cond; i++)
				{
					this->ColorPixel(current, {255, 0, 0});
			        current = came_from[current];
			        if(current == start)
			        {
			        	this->ColorPixel(current, {255, 0, 0});
			        }
			        if(queue.empty() || current == start)
						break;
		    	}
			}
	    }

	    if(saveResult)
	    {
	    	this->SavePicture("_dijkstra");
	    }
	}
	else
	{
		while(!queue.empty())
		{
			this->DijkstraStep(queue, came_from, cost_so_far);
		}
		while(current != start)
		{
			this->ColorPixel(current, {255, 0, 0});
			current = came_from[current];
		}
		this->ColorPixel(start, {255, 0, 0});

		this->SavePicture("_dijkstra");

	}
}

void Maze::DijkstraStep(std::priority_queue<WeightedPixel>& queue, 
	                   		std::unordered_map< Pixel, Pixel >& came_from,
	                   		std::unordered_map< Pixel, float>& cost_so_far)
{
	RGB grey = {126, 126, 126};
	RGB path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do DijkstraStep on an empty queue." );

	WeightedPixel coords = queue.top();
	Pixel current = {coords.x, coords.y};

	queue.pop();

	std::vector<Pixel> neighbors = this->GetNeighbors(current);

	for(Pixel& neighbor : neighbors)
	{
		float new_weight = cost_so_far[current] + this->GetWeightedCost(neighbor, current);

		//std::cout << "Checking: {" << neighbor.x << "," << neighbor.y << "}" << std::endl;

		if(neighbor == m_End)
		{
			//Break the loop
			std::cout << "Found end!" << std::endl;
			this->m_endFound = true;
			came_from.insert_or_assign(neighbor, current);
			cost_so_far.insert_or_assign(neighbor, new_weight);
			queue = std::priority_queue<WeightedPixel>();
			return;
		}
		else
		{
			this->ColorPixel(neighbor, grey);
			if(cost_so_far.count(neighbor) == 0 || new_weight < cost_so_far[neighbor])
			{
				cost_so_far.insert_or_assign(neighbor,new_weight);
				WeightedPixel neigh_weighted = {neighbor.x, neighbor.y, new_weight};
				queue.emplace(neigh_weighted);
				came_from.insert_or_assign(neighbor,current);
			}

		}
	}
}

void Maze::RunAStar(bool display, unsigned int scalar, bool saveResult)
{
	std::cout << "Running A*!" << std::endl;
	std::priority_queue<WeightedPixel> queue;

	std::unordered_map< Pixel, Pixel> came_from;
	std::unordered_map< Pixel, float> cost_so_far;

	came_from.insert_or_assign(this->m_Start, this->m_Start);
	cost_so_far.insert_or_assign(this->m_Start, 0);

	queue.emplace((WeightedPixel){this->m_Sx, this->m_Sy, 0});

	Pixel current = this->m_End;
	Pixel start = this->m_Start;

	this->m_endFound = false;

	if(display)
	{
		CImgDisplay main_disp((*this->img),"A* Search", 0);
		if(scalar == 0) 
			scalar = 1;

	    while (!main_disp.is_closed()) 
	    {
	    	main_disp.resize(500,500).display((*this->img));
			if(!queue.empty())
			{
				for(int i = 0; i < 1*scalar; i++)
				{
					this->AStarStep(queue, came_from, cost_so_far);
					if(queue.empty())
						break;
				}
			}
			else if(queue.empty() && current != start)
			{
				unsigned int cond = (1 * scalar/100);
				if (cond == 0)
					cond = 1;
				for(int i = 0; i < 1*cond; i++)
				{
					this->ColorPixel(current, {255, 0, 0});
			        current = came_from[current];
			        if(current == start)
			        {
			        	this->ColorPixel(current, {255, 0, 0});
			        }
			        if(queue.empty() || current == start)
						break;
		    	}
			}
	    }

	    if(saveResult)
	    {
	    	this->SavePicture("_astar");
	    }
	}
	else
	{
		while(!queue.empty())
		{
			this->AStarStep(queue, came_from, cost_so_far);
		}
		while(current != start)
		{
			this->ColorPixel(current, {255, 0, 0});
			current = came_from[current];
		}
		this->ColorPixel(start, {255, 0, 0});

		this->SavePicture("_astar");
	}
}

void Maze::AStarStep(std::priority_queue<WeightedPixel>& queue, 
	                   		std::unordered_map< Pixel, Pixel >& came_from,
	                   		std::unordered_map< Pixel, float>& cost_so_far)
{
	RGB grey = {126, 126, 126};
	RGB path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do DijkstraStep on an empty queue." );

	WeightedPixel coords = queue.top();
	Pixel current = {coords.x, coords.y};

	queue.pop();

	std::vector<Pixel> neighbors = this->GetNeighbors(current);

	for(Pixel& neighbor : neighbors)
	{
		float new_weight = cost_so_far[current] + this->GetWeightedCost(neighbor, current);

		if(neighbor == this->m_End)
		{
			//Break the loop
			std::cout << "Found end!" << std::endl;
			this->m_endFound = true;
			came_from.insert_or_assign(neighbor, current);
			cost_so_far.insert_or_assign(neighbor,new_weight);
			queue = std::priority_queue<WeightedPixel>();
			return;
		}
		else
		{
			this->ColorPixel(neighbor, grey);
			if((cost_so_far.count(neighbor) == 0) || (new_weight < cost_so_far[neighbor]))
			{
				cost_so_far.insert_or_assign(neighbor, new_weight);
				float priority = new_weight + this->GetHeuristicCost(m_End, neighbor);
				WeightedPixel neigh_weighted = {neighbor.x, neighbor.y, priority};
				queue.emplace(neigh_weighted);
				came_from.insert_or_assign(neighbor,current);
			}

		}
	}
}

void Maze::RunHPAStar(unsigned int clusterSize, unsigned int lvls, bool display, bool saveResult)
{
	HPAMaze hpamaze(this->img, lvls, clusterSize, this->m_Start, this->m_End);

	std::vector<std::vector<Cluster>> levels = hpamaze.GetClusters();

	RGB pink = {255, 0, 255};
	RGB red = {255, 0, 0};

	//this->ColorClusterIntraPaths(hpamaze, red, 1);

	//this->ColorClusterEntrances(hpamaze, pink, 1);

	std::unordered_map<Pixel, Pixel> path = hpamaze.AbstractPathfind(0);

	std::cout << "Getting color:" << (this->GetColor(30, 30)) << std::endl;

	//std::cout << "Path size: " << path.size() << std::endl;

	this->ColorHPAPath(hpamaze, red, path);

	this->SavePicture("_hpa");

}

void Maze::RunJPS(bool display, unsigned int scalar, bool saveResult)
{
	std::cout << "Running JPS!" << std::endl;

	std::priority_queue<WeightedPixel> queue;

	std::unordered_map< Pixel, Pixel> came_from;
	std::unordered_map< Pixel, float> cost_so_far;

	RGB path = {255, 0, 0};

	came_from.insert_or_assign(this->m_Start, this->m_Start);
	cost_so_far.insert_or_assign(this->m_Start, 0);

	queue.emplace((WeightedPixel){this->m_Sx, this->m_Sy, 0});

	Pixel current = this->m_End;
	Pixel start = this->m_Start;

	this->m_endFound = false;

	if(display)
	{
		CImgDisplay main_disp((*this->img),"JPS Search", 0);
		while (!main_disp.is_closed()) 
	    {
	    	main_disp.resize(500,500).display((*this->img));

			if(scalar == 0) 
				scalar = 1;

			if(!queue.empty())
			{
				for(int i = 0; i < 1*scalar; i++)
				{
					this->JPSStep(queue, came_from, cost_so_far);
					if(queue.empty())
						break;
				}
			}

			else if(queue.empty() && current != start)
			{
				unsigned int cond = (1 * scalar/100);
				if (cond == 0)
					cond = 1;
				for(int i = 0; i < 1*cond; i++)
				{
					this->ColorPixel(current, path);
			        current = came_from[current];
			        if(current == start)
			        {
			        	this->ColorPixel(current, path);
			        }
			        if(queue.empty() || current == start)
						break;
		    	}
			}
	    }

	    if(saveResult)
	    {
	    	this->SavePicture("_JPS");
	    }

	}
	else
	{


		while(!queue.empty() && this->m_endFound == false)
		{
			this->JPSStep(queue, came_from, cost_so_far);
		}
		while(current != start)
		{
			this->ColorPixel(current, path);
			current = came_from[current];
		}
		this->ColorPixel(start, path);

		this->SavePicture("_JPS");
	}

}

void Maze::JPSStep(std::priority_queue<WeightedPixel>& queue, 
	                   		std::unordered_map< Pixel, Pixel >& came_from,
	                   		std::unordered_map< Pixel, float>& cost_so_far){

	RGB purple = {255, 0, 255};
	RGB path = {255, 0, 0};

	if(queue.empty())
		throw std::invalid_argument( "Attempted to do JPS on an empty queue." );

	WeightedPixel coords = queue.top();
	Pixel current = {coords.x, coords.y};

	this->ColorPixel(current, purple);

	queue.pop();

	//std::cout << "\nRunning on next jump point" << current << std::endl; 
	std::vector<Pixel> neighbors = this->JPSPrunedNeighbors(current, came_from);
	
	for(Pixel neighbor : neighbors)
	{
		if(this->m_endFound == true)
			break;
		int dx = neighbor.x - current.x;
		int dy = neighbor.y - current.y;
		float new_weight = cost_so_far[current] + this->GetWeightedCost(neighbor, current);

		came_from.insert_or_assign(neighbor, current);
		cost_so_far.insert_or_assign(neighbor, new_weight);

		auto [n, JPFound] = this->JPSJump(neighbor, dx, dy, came_from, cost_so_far);

		if(JPFound)
		{
			//std::cout << "Found JP! Adding " << n << " to queue!" << std::endl;
			float priority = cost_so_far[n] + this->GetHeuristicCost(m_End, n);
			WeightedPixel neigh_weighted = {n.x, n.y, priority};
			queue.emplace(neigh_weighted);
		}
	}
}

std::tuple<Pixel, bool> Maze::JPSJump(Pixel& current,
										 int dx, 
										 int dy, 
										 std::unordered_map< Pixel, Pixel >& came_from,
										  std::unordered_map< Pixel, float>& cost_so_far)
{

	Pixel New = {current.x + dx, current.y + dy};
	//std::cout << "Jumping over " << current << " with dx: " << dx << " dy: " << dy << std::endl;


	if(current == this->m_End)
	{
		std::cout << "\n---Found end---\n" << std::endl;
		this->m_endFound = true;
		return std::tuple(current, true);
	}

	if(!this->IsWalkable(current))
	{
		//std::cout << "Pixel is not walkable" << std::endl;
		return std::tuple(current, false);
	}

	//Check for forced neighbours.
	if(dx != 0 && dy != 0) //Checking Diagonally
	{
		//std::cout << "Diagonal" << std::endl;
		if(!(current.x == 0 && dx == 1)
			&& !(current.y == 0 && dy == -1)
			&& !(current.x - dx == this->img->width())
			&& !(current.y + dy == this->img->height()))
		{
			if(!(this->IsWalkable(current.x - dx, current.y))
				&& this->IsWalkable(current.x - dx, current.y + dy))
			{
				//std::cout << "Added diagonal forced neighbor" << std::endl;
				return std::tuple(current, true);
			}
		}

		if(!(current.y == 0 && dy == 1)
			&& !(current.x == 0 && dx == -1)
			&& !(current.y - dy == this->img->height())
			&& !(current.x + dx == this->img->width()))
		{
			if(!(this->IsWalkable(current.x, current.y - dy))
				&& this->IsWalkable(current.x + dx, current.y - dy))
			{
				//std::cout << "Added diagonal forced neighbor" << std::endl;
				return std::tuple(current, true);
			}
		}

		auto [ __ , yt ] = this->JPSJump(current, 0, dy, came_from, cost_so_far);
		auto [ _ , xt ] = this->JPSJump(current, dx, 0, came_from, cost_so_far);

		if(xt || yt)
		{
			//std::cout << "Added " << current << " due to jump either xt or yt" << std::endl;
			return std::tuple(current, true);
		}

	}
	else if(dx != 0) //Horizontally
	{
		//std::cout << "Horizontally" << std::endl;

		if( current.y != 0
			&& !(current.x == 0 && dx == -1)
			&& !(current.x + dx == this->img->width() ))
		{
			if(!(this->IsWalkable(current.x, current.y - 1))
				&& this->IsWalkable(current.x + dx, current.y - 1))
			{
				//std::cout << "Added horizontal forced neighbor" << std::endl;
				return std::tuple(current, true);
			}
		}

		if( current.y != this->img->height()-1
			&& !(current.x == 0 && dx == -1)
			&& !(current.x + dx == this->img->width() ))
		{
			if(!(this->IsWalkable(current.x, current.y + 1))
				&& this->IsWalkable(current.x + dx, current.y + 1))
			{
				//std::cout << "Added horizontal forced neighbor" << std::endl;
				return std::tuple(current, true);
			}
		}

	}
	else //Vertically
	{
		//std::cout << "Vertically" << std::endl;

		if(current.x != 0
			&& !(current.y == 0 && dy == -1)
			&& !(current.y + dy == this->img->height()))
		{
			if(!this->IsWalkable(current.x - 1, current.y)
				&& this->IsWalkable(current.x - 1, current.y + dy))
			{
				//std::cout << "Added vertical forced neighbor" << std::endl;
				return std::tuple(current, true);
			}

		}

		if(current.x != this->img->width()-1
			&& !(current.y == 0 && dy == -1)
			&& !(current.y + dy == this->img->height()))
		{
			if(!this->IsWalkable(current.x + 1, current.y)
				&& this->IsWalkable(current.x + 1, current.y + dy))
			{
				//std::cout << "Added vertical forced neighbor" << std::endl;
				return std::tuple(current, true);
			}

		}

	}
	//std::cout << "Recursion" << std::endl;

	if(!(current.x == 0 && dx == -1)
		&& !(current.x + dx == this->img->width())
		&& !(current.y == 0 && dy == -1)
		&& !(current.y + dy == this->img->height()))
	{
		came_from.insert_or_assign(New, current);
		float new_weight = cost_so_far[current] + this->GetWeightedCost(New, current);
		cost_so_far.insert_or_assign(New, new_weight);
		return this->JPSJump(New, dx, dy, came_from, cost_so_far);
	}
	else
	{
		//std::cout << "Pixel is out of bounds" << std::endl;
		return std::tuple(current, false);
	}

}


std::vector<Pixel> Maze::JPSPrunedNeighbors(Pixel current, 
											std::unordered_map< Pixel, Pixel >& came_from){
	if(came_from[current] == current) //If we're at the start, return all directions. 
		return this->GetNeighbors(current); //TODO: Ensure optimality. We need to return in a specific order. 

	std::vector<Pixel> res;

	res.reserve(3);

	int dx = current.x - came_from[current].x;
    int dy = current.y - came_from[current].y;

    //std::cout << "Getting pruned for " << current << " dx: " << dx << " dy: " << dy << std::endl;

    bool debug = false;


    if(dy == 0) //We're moving horizontally.
    {
    	if(!(dx == -1 && current.x == 0)
    		&& !(current.x+dx == this->img->width()) )
    	{
    		Pixel Next = {current.x + dx, current.y};
    		if(this->IsWalkable(Next) || Next == this->m_End)
    		{
    			res.emplace_back(Next);
    		}
			//Is there a forced neighbour above?
			if(current.y != 0 && !this->IsWalkable(current.x, current.y - 1))
			{
				Pixel Forced = {current.x + dx, current.y - 1};
				if(this->IsWalkable(Forced) || Forced == this->m_End)
					res.emplace_back(Forced);
			} 
			//Is there a forced neighbour below?
			if(current.y != this->img->height()-1 && !this->IsWalkable(current.x, current.y + 1))
			{
				Pixel Forced = {current.x + dx, current.y + 1};
				if(this->IsWalkable(Forced) || Forced == this->m_End)
					res.emplace_back(Forced);
			}
    	}
    }
    else if(dx == 0) //We're moving vertically.
    {
    	if(!(dy == -1 && current.y == 0)
    		&& !(current.y + dy == this->img->height()) )
    	{
    		Pixel Next = {current.x, current.y + dy};
    		if(this->IsWalkable(Next) || Next == this->m_End)
    		{
    			res.emplace_back(Next);
    		}
    		//Is there a forced neighbour to the left?
			if(current.x != 0 && !this->IsWalkable(current.x - 1, current.y))
			{
				Pixel Forced = {current.x-1, current.y + dy};
				if(this->IsWalkable(Forced) || Forced == this->m_End)
					res.emplace_back(Forced);
			}
			//Is there a forced neighbour to the right?
			if (current.x != this->img->width()-1 && !this->IsWalkable(current.x + 1, current.y))
			{
				Pixel Forced = {current.x+1, current.y + dy};
				if(this->IsWalkable(Forced) || Forced == this->m_End)
					res.emplace_back(Forced);
			}
    	}

    }
    else //We're moving diagonally
    {
    	bool _withinYAxis, _withinXAxis = false; //Bounds check for our 3rd pixel

    	if(!(dy == -1 && current.y == 0)
    		&& !(current.y + dy == this->img->height()))
    	{
    		_withinYAxis = true;
    		Pixel Next = {current.x, current.y + dy};
    		if(this->IsWalkable(Next) || Next == this->m_End)
    		{
    			res.emplace_back(Next);
    		}
    	}
    	if(!(dx == -1 && current.x == 0)
    		&& !(current.x + dx == this->img->width()) )
    	{
    		_withinXAxis = true;
    		Pixel Next = {current.x + dx, current.y};
    		if(this->IsWalkable(Next) || Next == this->m_End)
    		{
    			res.emplace_back(Next);
    		}
    	}

    	if(_withinYAxis && _withinXAxis)
    	{
    		Pixel Next = {current.x + dx, current.y + dy};
    		if(this->IsWalkable(Next) || Next == this->m_End)
    		{
    			res.emplace_back(Next);
    		}
    	}

    	//Do we have a forced neighbour?
    	//Forced neighbours on a diagonal are always diagonals. Therefore,
    	//They are added last. 
    	if(!(dx == 1 && current.x == 0)
    		&& !(current.x - dx == this->img->width())
    		&& _withinYAxis
    		&& !this->IsWalkable(current.x - dx, current.y))
    	{
			Pixel Forced = {current.x - dx, current.y + dy};
    		if(this->IsWalkable(Forced) || Forced == this->m_End)
    		{
    			res.emplace_back(Forced);
    		}
    	}

    	if(!(dy == 1 && current.y == 0)
    		&& !(current.y - dy == this->img->height())
    		&& _withinXAxis
    		&& !this->IsWalkable(current.x, current.y - dy))
    	{
			Pixel Forced = {current.x + dx, current.y - dy};
    		if(this->IsWalkable(Forced) || Forced == this->m_End)
    		{
    			res.emplace_back(Forced);
    		}
    	}

    }
    /*
    std::cout << "Res: " << res.size() << std::endl;

    for(Pixel t : res)
    	std::cout << " Pixel: " << t;
    std::cout << std::endl;*/
	return res;
}

void Maze::ColorClusterEntrances(HPAMaze& hpamaze, RGB& color, unsigned int lvl)
{
	std::vector<std::vector<Cluster>> levels = hpamaze.GetClusters();
	if(lvl > levels.size() || lvl == 0)
		throw std::invalid_argument( "Attempted to do ColorClusterEntrances on a lvl that does not exist" );

	std::cout << "Coloring clusters in level " << lvl << std::endl;
	for (const std::vector<Cluster>& level : levels)
	{
		for (const Cluster& cluster : level)
		{
			for (const auto& p : cluster.trans ) 
			{
		        for(const Edge& ed : p.second)
		        {
		        	if(ed.type == Edge::EdgeType::INTER)
		        	{
			        	this->ColorPixel(ed.s, color);
			        	this->ColorPixel(ed.e, color);
			        }
		        }
			}
		}

	}

}

void Maze::ColorClusterIntraPaths(HPAMaze& hpamaze, RGB& color, unsigned int lvl)
{
	std::vector<std::vector<Cluster>> levels = hpamaze.GetClusters();
	if(lvl > levels.size() || lvl == 0)
		throw std::invalid_argument( "Attempted to do ColorClusterEntrances on a lvl that does not exist" );

	std::cout << "Coloring intra paths in level " << lvl << std::endl;

	unsigned int currentLvl = 1;
	for (std::vector<Cluster> level : levels)
	{
		if(currentLvl != lvl)
			continue;
		currentLvl++;
		for (Cluster cluster : level)
		{			
			for (auto const& p : cluster.trans ) 
			{
		        for(Edge ed : p.second)
		        {
		        	if(ed.type == Edge::EdgeType::INTRA)
		        	{
		        		Pixel current = ed.e;
		        		while(current != ed.s)
		        		{
		        			this->ColorPixel(current, color);
							current = ed.path[current];
		        		}
		        	}
		        }
			}
		}

	}

}

void Maze::ColorHPAPath(HPAMaze& hpamaze, RGB& color, std::unordered_map<Pixel, Pixel>& path)
{
	for(const auto& p: path)
	{
		this->ColorPixel(p.first, color);
		this->ColorPixel(p.second, color);

	}

	//this->ColorPixel(this->m_End, color);
}


float Maze::GetHeuristicCost(Pixel goal, Pixel current)
{
	//Euclidean Distance
	// sqrt(|x2 - x1|^2 + |y2 - y1|^2)

	unsigned long long res = 0;
	//OBS - We are open to an overflow error.
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

float Maze::GetHeuristicManhattenCost(Pixel goal, Pixel current)
{
	// Manhatten distance
	// |x2 - x1| + |y2 - y1|
	float res = 0; 
	if(goal.x >= current.x)
	{
		  res += goal.x - current.x;
	}
	else
	{
		res += current.x - goal.x;
	}

	if(goal.y >= current.y)
	{
		res += goal.y - current.y;
	}
	else
	{
		res += current.y - goal.y;
	}
	return res;

}

float Maze::GetWeightedCost(Pixel neighbor, Pixel current)
{
	float cost = 0.0f;
	if(neighbor == current)
		return cost;

	//The cost to move in a straight line is 1, while diagonally is sqrt(2)
	if((current.x - neighbor.x == 0) || (current.y - neighbor.y == 0))
		cost = 1.0f;
	else
		cost = SQRT2;

	return cost;
}


void Maze::Test()
{

	

}

std::vector<Pixel> Maze::GetNeighbors(Pixel& coords, bool FindEightNeighbors)
{
	return this->GetNeighbors(coords.x, coords.y, FindEightNeighbors);
}

std::vector<Pixel> Maze::GetNeighbors(unsigned int x, unsigned int y, bool FindEightNeighbors)
{
	std::vector<Pixel> result;

	if(FindEightNeighbors)
	{
		result.reserve(8);

		//We need to return horizontal - and vertical neighbours first to ensure
		//JPS optimality.
		if(!(y == this->img->height()-1))
		{
			Pixel n = {x , y + 1};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		if(!(y == 0))
		{
			Pixel n = {x, y - 1};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		if(!(x == this->img->width()-1))
		{
			Pixel n = {x + 1, y};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		if(!(x == 0))
		{
			Pixel n = {x - 1, y};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		//Diagonal neighbours.
		if(!(x == 0) && !(y == 0))
		{
			Pixel n = {x - 1, y - 1};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		if(!(x == this->img->width()-1) && !(y == 0))
		{
			Pixel n = {x + 1, y - 1};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		if(!(x == 0) && !(y == this->img->height()-1))
		{
			Pixel n = {x - 1, y + 1};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		if(!(x == this->img->width()-1) && !(y == this->img->height()-1))
		{
			Pixel n = {x + 1, y + 1};
			if(this->IsWalkable(n) || n == this->m_End)
				result.emplace_back(n);
		}

		return result;
	}
	else
	{
		result.reserve(4);	
		Pixel neighbor;

		//An important thing to note is that neighbors are pushed to the back of the vector - aka first in, last out.
		if(x != (this->img->width()-1)) //Down
		{
			neighbor = {x + 1, y};
			if(this->IsWalkable(neighbor))
			{
				result.emplace_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.emplace_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
		if(x != 0) //Up
		{
			neighbor = {x - 1, y};
			if(this->IsWalkable(neighbor))
			{
				result.emplace_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.emplace_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
		if(y != (this->img->height()-1)) //Right
		{
			neighbor = {x, y+1};
			if(this->IsWalkable(neighbor))
			{
				result.emplace_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.emplace_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
		if(y != 0) //Left
		{
			neighbor = {x, y-1};
			if(this->IsWalkable(neighbor))
			{
				result.emplace_back(neighbor);
			}
			else if(neighbor.x == this->m_Ex && neighbor.y == this->m_Ey)
			{
				result.emplace_back(neighbor);
				return result; //If we have found the end, return early.
			}
		}
	}

	return result;

}

void Maze::SavePicture(std::string filename)
{
	std::string saveName = this->exampleFolder + this->m_name + filename + ".png";
	this->img->save(saveName.c_str());
}

void Maze::RunAll()
{
	//TODO
	//Currently doesn't function as the picture is overwritten when loaded into memory.
	//this->RunBreadth();
	//this->RunDijkstra();
	//this->RunAStar();

	std::cout << "TODO: RunAll()" << std::endl;
}

}//End of namespace Eng
