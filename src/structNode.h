#pragma once

#include <iostream>
#include "classEdge.h"
#include "structPixels.h"

namespace Eng
{

	/**
     * A structure to represent pixels with a list of all the edges going out from that pixels
     */
	struct Node{
		Pixel p;
		std::vector<Edge> edges;

		bool operator ==(const Node& rhs) const
		{
			return (this->p == rhs.p);
		};

		bool operator !=(const Node& rhs) const
		{
			return (this->p != rhs.p);
		};
		bool operator <(const Node& rhs) const
		{
			return this->p < rhs.p;
		}
	};

}//End of namespace Eng



