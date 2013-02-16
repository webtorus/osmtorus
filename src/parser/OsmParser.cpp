#include <cstring>
#include <iostream>
#include <map>

#include "OsmParser.hpp"

using namespace std;


OsmParser::OsmParser()
{
	current_element = EMPTY;
	
	// Initialiser tous les pointeurs de callback à NULL
	memset(&sax_handler, 0, sizeof(xmlSAXHandler));

	// Spécifier ceux utiles
	sax_handler.startDocument = OsmParser::onStartDocument;
	sax_handler.endDocument = OsmParser::onEndDocument;
	sax_handler.startElement = OsmParser::onStartElement;
	sax_handler.endElement = OsmParser::onEndElement;
}

OsmParser::~OsmParser()
{
	map<long, ParsedNode*>::iterator it_n;
	for (it_n = nodes.begin(); it_n != nodes.end(); it_n++) {
		ParsedNode* node = it_n->second;
		delete node;
	}

	map<long, ParsedWay*>::iterator it_w;
	for (it_w = ways.begin(); it_w != ways.end(); it_w++) {
		ParsedWay* way = it_w->second;
		delete way;
	}

	map<long, ParsedRelation*>::iterator it_r;
	for (it_r = relations.begin(); it_r != relations.end(); it_r++) {
		ParsedRelation* relation = it_r->second;
		delete relation;
	}
}

map<long, ParsedNode*>& OsmParser::getNodes()
{
	return nodes;
}

map<long, ParsedWay*>& OsmParser::getWays()
{
	return ways;
}

map<long, ParsedRelation*>& OsmParser::getRelations()
{
	return relations;
}

void OsmParser::parseFile(string file_path)
{
	xmlSAXUserParseFile(&sax_handler, this, file_path.c_str());
}

void OsmParser::onStartDocument(void* parser)
{
	OsmParser* self = static_cast<OsmParser*>(parser);
	self->initialize();
}

void OsmParser::onEndDocument(void* parser)
{
	OsmParser* self = static_cast<OsmParser*>(parser);
	self->finish();
}

void OsmParser::initialize()
{
	boxes.min_x = 180;
	boxes.min_y = 180;
	boxes.max_x = -180;
	boxes.max_y = -180;
}

void OsmParser::finish()
{

}

void OsmParser::onStartElement(void* parser, const xmlChar* name, const xmlChar** attrs)
{
	OsmParser* self = static_cast<OsmParser*>(parser);

	if (xmlStrEqual(name, BAD_CAST "node")) {
		self->parseNode(attrs);
	} else if (xmlStrEqual(name, BAD_CAST "way")) {
		self->parseWay(attrs);
	} else if (xmlStrEqual(name, BAD_CAST "relation")) {
		self->parseRelation(attrs);
	} else if (xmlStrEqual(name, BAD_CAST "nd")) {
		self->parseNodeWay(attrs);
	} else if (xmlStrEqual(name, BAD_CAST "member")) {
		self->parseMemberRelation(attrs);
	} else if (xmlStrEqual(name, BAD_CAST "tag")) {
		self->parseTag(attrs);
	}
}

void OsmParser::onEndElement(void* parser, const xmlChar* name)
{
	OsmParser* self = static_cast<OsmParser*>(parser);

	if (xmlStrEqual(name, BAD_CAST "way")) {
		self->validCurrentWay();
	} else if (xmlStrEqual(name, BAD_CAST "node")) {
		self->validCurrentNode();
	} else if (xmlStrEqual(name, BAD_CAST "relation")) {
		self->validCurrentRelation();
	}
}

void OsmParser::parseTag(const xmlChar** attrs)
{
	if (NODE == current_element) {
		parseTagNode(attrs);
	} else if (WAY == current_element) {
		parseTagWay(attrs);
	} else if (RELATION == current_element) {
		parseTagRelation(attrs);
	}
}

void OsmParser::parseNode(const xmlChar** attrs)
{
	current_element = NODE;
	ParsedNode* node = new ParsedNode();

	for (int i = 0; attrs[i] != NULL; i += 2) {
		if (xmlStrEqual(attrs[i], BAD_CAST "id")) {
			node->id = atol((char*) attrs[i + 1]);
		} else if (xmlStrEqual(attrs[i], BAD_CAST "lat")) {
			node->lat = atof((char*) attrs[i + 1]);
		} else if (xmlStrEqual(attrs[i], BAD_CAST "lon")) {
			node->lon = atof((char*) attrs[i + 1]);
		}
	}

	if (node->lat < boxes.min_x) {
		boxes.min_x = node->lat;
	}
	if (node->lat > boxes.max_x) {
		boxes.max_x = node->lat;
	}
	if (node->lon < boxes.min_y) {
		boxes.min_y = node->lon;
	}
	if (node->lon > boxes.max_y) {
		boxes.max_y = node->lon;
	}

	current_node = node;
}

void OsmParser::parseTagNode(const xmlChar** attrs)
{

}

void OsmParser::validCurrentNode()
{
	current_element = EMPTY;

	current_node = NULL;
}

void OsmParser::parseWay(const xmlChar** attrs)
{
	current_element = WAY;
	ParsedWay* way = new ParsedWay();
	for (int i = 0; attrs[i] != NULL; i += 2) {
		if (xmlStrEqual(attrs[i], BAD_CAST "id")) {
			way->id = atol((char*) attrs[i + 1]);
		}
	}

	current_way = way;
}

void OsmParser::parseNodeWay(const xmlChar** attrs)
{

}


void OsmParser::parseTagWay(const xmlChar** attrs)
{

}

void OsmParser::validCurrentWay()
{
	current_element = EMPTY;
	
	current_way = NULL;
}

void OsmParser::parseRelation(const xmlChar** attrs)
{
	current_element = RELATION;
	ParsedRelation* relation = new ParsedRelation();
	
	for (int i = 0; attrs[i] != NULL; i += 2) {
		if (xmlStrEqual(attrs[i], BAD_CAST "id")) {
			relation->id = atol((char*) attrs[i + 1]);
		}
	}

	current_relation = relation;
}

void OsmParser::parseMemberRelation(const xmlChar** attrs)
{

}

void OsmParser::parseTagRelation(const xmlChar** attrs)
{

}

void OsmParser::validCurrentRelation()
{
	current_element = EMPTY;

	current_relation = NULL;
}