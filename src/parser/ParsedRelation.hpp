#ifndef PARSED_RELATION_HPP
#define PARSED_RELATION_HPP

#include <map>

#include "ParsedNode.hpp"
#include "ParsedWay.hpp"

using namespace std;

enum RelationType
{
	OTHER,
	TRAM,
	BUS
};

class ParsedNode;
class ParsedWay;

class ParsedRelation
{
public:
	ParsedRelation() : id(0), type(OTHER)
	{
	}

	long id;
	short type;
	string ref;
	string color;
	map<long, ParsedWay*> ways;
	map<long, ParsedNode*> nodes;
};

#endif
