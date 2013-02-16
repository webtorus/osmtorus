#ifndef EDGE_PROCESSOR_HPP
#define EDGE_PROCESSOR_HPP

#include <map>

#include "ParsedNode.hpp"
#include "ParsedWay.hpp"
#include "ParsedRelation.hpp"

using namespace std;

class EdgeProcessor
{
public:
	EdgeProcessor(map<long, ParsedNode>& nodes, map<long, ParsedWay>& ways, map<long, ParsedRelation>& relations);
	// map<long, Node>& getNodes();
	// map<long, Way>& getWays();
	virtual void optimizeForRouting();

protected:
	virtual void processNodes();
	virtual void processWays();
	virtual void processRelations();

private:
	// map<long, Node> nodes;
	// map<long, Way> ways;

	map<long, ParsedNode> parsedNodes;
	map<long, ParsedWay> parsedWays;
	map<long, ParsedRelation> parsedRelations;
};

#endif