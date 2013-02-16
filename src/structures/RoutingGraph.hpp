#ifndef ROUTING_GRAPH_HPP
#define ROUTING_GRAPH_HPP

#include <map>
#include <set>
#include "Node.hpp"
#include "Way.hpp"
#include "Edge.hpp"
#include "TransportLine.hpp"
#include "BoundingBoxes.hpp"

using namespace std;

class RoutingGraph
{
public:
	RoutingGraph()
	{
	}

	~RoutingGraph()
	{
		map<long, Node*>::iterator it_n;
		for (it_n = nodes.begin(); it_n != nodes.end(); it_n++) {
			Node* node = it_n->second;
			delete node;
		}

		set<Edge*>::iterator it_e;
		for (it_e = edges.begin(); it_e != edges.end(); it_e++) {
			Edge* edge = *it_e;
			delete edge;
		}

		map<long, Way*>::iterator it_w;
		for (it_w = ways.begin(); it_w != ways.end(); it_w++) {
			Way* way = it_w->second;
			delete way;
		}

		map<long, TransportLine*>::iterator it_tl;
		for (it_tl = transport_lines.begin(); it_tl != transport_lines.end(); it_tl++) {
			TransportLine* line = it_tl->second;
			delete line;
		}

		nodes.clear();
		ways.clear();
		transport_lines.clear();
		edges.clear();
	}

	map<long, Node*> nodes;
	map<long, Way*> ways;
	map<long, TransportLine*> transport_lines;
	set<Edge*> edges;

	BoundingBoxes<Node*, 50, 50> boxes;
};

#endif
