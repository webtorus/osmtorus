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
		Edge();

		static unsigned int distanceBetween(Node* n1, Node* n2);

		unsigned int distance;

		Node* to;
		Way* way;
};

bool operator<(const Edge& e1, const Edge& e2);

#endif
