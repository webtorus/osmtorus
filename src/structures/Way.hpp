#ifndef WAY_HPP
#define WAY_HPP

#include <vector>

#include "Node.hpp"

using namespace std;

class Node;

enum WayType
{
	FootWay = 1,
	CycleWay = 2,
	CarWay = 4,
	TramWay = 8
};

class Way
{
	public:
		Way() : id(0), type(0), name("")
		{
		}

		long id;
		short type;
		string name;
};

#endif
