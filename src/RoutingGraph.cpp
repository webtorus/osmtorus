#include "include/RoutingGraph.hpp"
#include "include/Node.hpp"
#include "include/Edge.hpp"
#include "include/TransportLine.hpp"
#include "include/Way.hpp"
#include <map>
#include <set>

using namespace std;

RoutingGraph::RoutingGraph()
{

}

RoutingGraph::~RoutingGraph()
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
