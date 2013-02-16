#ifndef NODE_HPP
#define NODE_HPP

#include <set>
#include <map>

#include "Edge.hpp"
#include "Way.hpp"

using namespace std;

class Edge;
class Way;

class Node
{
public:
	Node(): id(0), lat(0), lon(0), bus_stop(false), tram_stop(false)
	{
	}

	long id;
	double lat;
	double lon;
	bool bus_stop;
	bool tram_stop;
	string name;
	set<Edge*> neighbors;
	map<long, Way*> ways;
};

#endif
