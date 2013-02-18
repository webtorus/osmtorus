#ifndef EDGE_HPP
#define EDGE_HPP

#include "Node.hpp"
#include "Way.hpp"

class Node;
class Way;

class Edge
{
	public:
		Edge();

		unsigned int length;
		Node* from;
		Node* to;
		Way* way;
};

#endif
