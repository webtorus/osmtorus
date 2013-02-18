#ifndef OSM_PARSER_H
#define OSM_PARSER_H

#include "BoundingBoxes.hpp"
#include "ParsedNode.hpp"
#include "ParsedRelation.hpp"
#include "ParsedWay.hpp"
#include <libxml/parser.h>
#include <map>
#include <string>

#define K 1
#define V 3

using namespace std;

enum CurrentElementType
{
	EMPTY,
	NODE,
	WAY,
	RELATION
};

class OsmParser
{
	public:
		OsmParser();
		virtual ~OsmParser();
		virtual void parseFile(string file_path);
		virtual map<long, ParsedNode*>& getNodes();
		virtual map<long, ParsedWay*>& getWays();
		virtual map<long, ParsedRelation*>& getRelations();

	protected:
		static void onStartDocument(void* parser);
		static void onEndDocument(void* parser);
		static void onStartElement(void* parser, const xmlChar* name, const xmlChar** attrs);
		static void onEndElement(void* parser, const xmlChar* name);

	protected:
		virtual void initialize();
		//virtual void parseTag(const xmlChar** attrs);
		virtual void parseNode(const xmlChar** attrs);
		//virtual void parseTagNode(const xmlChar** attrs);
		virtual void validCurrentNode();
		virtual void parseWay(const xmlChar** attrs);
		//virtual void parseNodeWay(const xmlChar** attrs);
		virtual void validCurrentWay();
		//virtual void parseTagWay(const xmlChar** attrs);
		virtual void parseRelation(const xmlChar** attrs);
		//virtual void parseMemberRelation(const xmlChar** attrs);
		//virtual void parseTagRelation(const xmlChar** attrs);
		virtual void validCurrentRelation();
		virtual void finish();

		xmlSAXHandler sax_handler;

		CurrentElementType current_element;

		ParsedWay* current_way;
		ParsedNode* current_node;
		ParsedRelation* current_relation;

		BoundingBoxes<ParsedNode*, 50, 50> boxes;
	
		map<long, ParsedRelation*> relations;
		map<long, ParsedNode*> nodes;
		map<long, ParsedWay*> ways;
};

#endif 
