#ifndef group_edge_HPP
#define group_edge_HPP

#include "Edge.hpp"
#include "TransportLine.hpp"
#include <list>
#include <map>

class EdgeGroup
{
	public:
		EdgeGroup();

		std::map<long, TransportLine*> transport_lines;
		std::list<Edge*> edges;
};

#endif
