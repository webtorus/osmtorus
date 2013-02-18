#ifndef ROUTING_GRAPH_HPP
#define ROUTING_GRAPH_HPP

#include "BoundingBoxes.hpp"
#include "Edge.hpp"
#include "Node.hpp"
#include "TransportLine.hpp"
#include "Way.hpp"
#include <map>
#include <set>

using namespace std;

class RoutingGraph
{
	public:
		RoutingGraph();
		~RoutingGraph();

		map<long, Node*> nodes;
		map<long, Way*> ways;
		map<long, TransportLine*> transport_lines;
		set<Edge*> edges;
		BoundingBoxes<Node*, 50, 50> boxes;
};

#endif
