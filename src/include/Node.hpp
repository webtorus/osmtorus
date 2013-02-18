#ifndef NODE_HPP
#define NODE_HPP

#include "Edge.hpp"
#include "Way.hpp"
#include <map>
#include <set>
#include <string>

using namespace std;

class Edge;
class Way;

class Node
{
	public:
		Node();

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
