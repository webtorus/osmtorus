#ifndef ROUTING_OSM_PARSER_H
#define ROUTING_OSM_PARSER_H

#include <libxml/parser.h>
#include <map>

#include "OsmParser.hpp"
#include "../structures/RoutingGraph.hpp"

class RoutingOsmParser: public virtual OsmParser
{
public:
	RoutingOsmParser();
	virtual ~RoutingOsmParser();

	static short highwayType(const xmlChar* type);
	static short relationRouteType(const xmlChar* type);

	virtual RoutingGraph* createRoutingGraph();

protected:
	virtual void parseTag(const xmlChar** attrs);
	virtual void parseNode(const xmlChar** attrs);
	virtual void parseTagNode(const xmlChar** attrs);
	virtual void validCurrentNode();
	virtual void parseWay(const xmlChar** attrs);
	virtual void parseNodeWay(const xmlChar** attrs);
	virtual void validCurrentWay();
	virtual void parseTagWay(const xmlChar** attrs);
	virtual void parseRelation(const xmlChar** attrs);
	virtual void parseMemberRelation(const xmlChar** attrs);
	virtual void parseTagRelation(const xmlChar** attrs);
	virtual void validCurrentRelation();
	virtual void finish();

private:
	virtual void deleteUselessNodes();
	virtual void linkBusStopsToBusNetwork();
	virtual void linkStopsToNetwork();
	virtual void linkBusStop(ParsedNode* bus_stop, ParsedRelation* relation);
	virtual void linkStop(ParsedNode* stop);

	static double distanceFromLine(const ParsedNode* point, const ParsedNode* n1, const ParsedNode* n2, ParsedNode* result);
	static unsigned int distanceBetween(const ParsedNode* n1, const ParsedNode* n2);
};


#endif 