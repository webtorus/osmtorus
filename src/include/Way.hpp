#ifndef WAY_HPP
#define WAY_HPP

#include "TransportLine.hpp"
#include "WayType.hpp"
#include <map>
#include <string>

using namespace std;

class Node;

class Way
{
	public:
		Way();

		long id;
		short type;
		string name;
		map<long, TransportLine*> transport_lines;
};

#endif
