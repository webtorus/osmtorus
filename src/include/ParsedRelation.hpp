#ifndef PARSED_RELATION_HPP
#define PARSED_RELATION_HPP

#include "ParsedNode.hpp"
#include "ParsedWay.hpp"
#include <map>
#include <string>

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
		ParsedRelation();

		long id;
		short type;
		string ref;
		string color;
		map<long, ParsedWay*> ways;
		map<long, ParsedNode*> nodes;
};

#endif
