#ifndef PARSED_NODE_HPP
#define PARSED_NODE_HPP

#include "ParsedRelation.hpp"
#include "ParsedWay.hpp"
#include <map>
#include <string>

using namespace std;

class ParsedRelation;
class ParsedWay;

class ParsedNode
{
	public:
		ParsedNode();

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
