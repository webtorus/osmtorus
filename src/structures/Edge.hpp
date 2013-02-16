#ifndef EDGE_HPP
#define EDGE_HPP

using namespace std;

#include <cmath>
#include "Node.hpp"
#include "Way.hpp"

class Node;
class Way;

class Edge
{
	public:
		Edge(): length(0), to(NULL), way(NULL)
		{
		}

		unsigned int length;
		Node* from;
		Node* to;
		Way* way;
};

#endif
