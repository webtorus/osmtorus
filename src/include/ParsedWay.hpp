#ifndef PARSED_WAY_HPP
#define PARSED_WAY_HPP

#include "ParsedNode.hpp"
#include "ParsedRelation.hpp"
#include "WayType.hpp"
#include <map>
#include <string>
#include <vector>

using namespace std;

class ParsedNode;
class ParsedRelation;

class ParsedWay
{
	public:
		ParsedWay();

		long id;
		short type;
		string name;
		vector<ParsedNode*> path;
		map<long, ParsedRelation*> relations;
};

#endif
