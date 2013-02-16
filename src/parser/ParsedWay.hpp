#ifndef PARSED_WAY_HPP
#define PARSED_WAY_HPP

#include <vector>
#include <map>

#include "../structures/WayType.hpp"
#include "ParsedNode.hpp"
#include "ParsedRelation.hpp"

using namespace std;

class ParsedNode;
class ParsedRelation;

class ParsedWay
{
public:
	ParsedWay() : id(0), type(0)
	{
	}

	long id;
	short type;
	string name;
	vector<ParsedNode*> path;
	map<long, ParsedRelation*> relations;
};

#endif
