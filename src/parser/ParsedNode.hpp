#ifndef PARSED_NODE_HPP
#define PARSED_NODE_HPP

#include <map>
#include <string>

#include "ParsedRelation.hpp"
#include "ParsedWay.hpp"

using namespace std;

class ParsedRelation;
class ParsedWay;

class ParsedNode
{
public:
	ParsedNode(): id(0), lat(0), lon(0), bus_stop(false), tram_stop(false)
	{
	}

	long id;
	double lat;
	double lon;
	bool bus_stop;
	bool tram_stop;
	string name;
	map<long, ParsedRelation*> relations;
	map<long, ParsedWay*> ways;
};

#endif
