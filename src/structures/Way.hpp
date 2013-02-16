#ifndef WAY_HPP
#define WAY_HPP

#include <map>
#include "WayType.hpp"
#include "TransportLine.hpp"

using namespace std;

class Node;

class Way
{
public:
	Way() : id(0), type(0)
	{
	}

	long id;
	short type;
	string name;
	map<long, TransportLine*> transport_lines; 
};

#endif
