#ifndef LINE_EDGES_HPP
#define LINE_EDGES_HPP

#include "Edge.hpp"
#include "TransportLine.hpp"
#include <list>

class LineEdges
{
	public:
		LineEdges();

		TransportLine* transport_line;
		std::list<Edge*> edges;
};

#endif
