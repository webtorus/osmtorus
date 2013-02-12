#ifndef OSM_PARSER_H
#define OSM_PARSER_H

#include <libxml/parser.h>
#include <map>

#include "../structures/Node.hpp"
#include "../structures/Way.hpp"
#include "../structures/Edge.hpp"

class OsmParser
{
public:
	OsmParser();
	virtual ~OsmParser();
	virtual void parseFile(std::string file_path);
	virtual map<long, Node*>& getNodes();
	virtual map<long, Way*>& getWays();

	static int highwayType(const xmlChar* type);

protected:
	static void onStartDocument(void* parser);
	static void onEndDocument(void* parser);
	static void onStartElement(void* parser, const xmlChar* name, const xmlChar** attrs);
	static void onEndElement(void* parser, const xmlChar* name);

private:
	void parseNode(const xmlChar** attrs);
	void parseWay(const xmlChar** attrs);
	void parseNodeWay(const xmlChar** attrs);
	void parseTagNode(const xmlChar** attrs);
	void parseTagWay(const xmlChar** attrs);
	void validCurrentWay();
	void deleteUseless();

	xmlSAXHandler sax_handler;

	const xmlChar* current;
	Way* current_way;
	vector<Node*> current_way_nodes;
	Node* current_node;


	map<long, Node*> nodes;
	map<long, Way*> ways;
};


#endif 