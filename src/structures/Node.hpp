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
		Node(): id(0), lat(0), lon(0)
		{
		}

		long id;
		double lat;
		double lon;
		set<Edge*> neighbors;
		map<long, Way*> ways;
};

#endif
