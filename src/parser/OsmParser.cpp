#include <cstring>
#include <iostream>

#include "OsmParser.hpp"

using namespace std;


OsmParser::OsmParser()
{
	current = BAD_CAST "";
	
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
}

void OsmParser::onStartDocument(void* parser)
{
}

void OsmParser::onEndDocument(void* parser)
{
	OsmParser* self = static_cast<OsmParser*>(parser);

	self->deleteUseless();
}

void OsmParser::deleteUseless()
{
	cout << "Nombre de voies : " << ways.size() << endl;

	cout << "Avant suppression des noeuds inutiles : " << nodes.size() << endl;
	// Supprime tous les noeuds qui ne sont pas dans une voie
	map<long, Node*>::iterator it_n = nodes.begin();

	while (it_n != nodes.end()) {
		if (NULL == it_n->second || it_n->second->ways.empty()) {
			delete it_n->second;
			nodes.erase(it_n++);
		} else {
			it_n++;
		}
	}

	cout << "Après suppression des noeuds inutiles : " << nodes.size() << endl;
}

void OsmParser::parseFile(string file_path)
{
	xmlSAXUserParseFile(&sax_handler, this, file_path.c_str());
}

map<long, Node*>& OsmParser::getNodes()
{
	return nodes;
}

map<long, Way*>& OsmParser::getWays()
{
	return ways;
}

void OsmParser::onStartElement(void* parser, const xmlChar* name, const xmlChar** attrs)
{
	OsmParser* self = static_cast<OsmParser*>(parser);

	if (xmlStrEqual(name, BAD_CAST "node")) {
		self->parseNode(attrs);
		self->current = name;
	} else if (xmlStrEqual(name, BAD_CAST "way")) {
		self->parseWay(attrs);
		self->current = name;
	} else if (xmlStrEqual(name, BAD_CAST "nd")) {
		self->parseNodeWay(attrs);
	} else if (xmlStrEqual(name, BAD_CAST "tag")) {
		if  (xmlStrEqual(self->current, BAD_CAST "node")) {
			self->parseTagNode(attrs);
		} else if (xmlStrEqual(self->current, BAD_CAST "way")) {
			self->parseTagWay(attrs);
		}
	}
}

void OsmParser::onEndElement(void* parser, const xmlChar* name)
{
	OsmParser* self = static_cast<OsmParser*>(parser);

	if (xmlStrEqual(name, BAD_CAST "way")) {
		self->validCurrentWay();
		self->current = BAD_CAST "";
	} else if (xmlStrEqual(name, BAD_CAST "node")) {
		self->current = BAD_CAST "";
	}
}

void OsmParser::parseTagNode(const xmlChar** attrs)
{

}

int OsmParser::highwayType(const xmlChar* type)
{
	if (xmlStrEqual(type, BAD_CAST "path")) {
		return FootWay|CycleWay;
	} else if (xmlStrEqual(type, BAD_CAST "pedestrian")) {
		return FootWay;
	} else if (xmlStrEqual(type, BAD_CAST "footway")) {
		return FootWay;
	} else if (xmlStrEqual(type, BAD_CAST "cycleway")) {
		return CycleWay;
	} else if (xmlStrEqual(type, BAD_CAST "living_street")) {
		return FootWay|CycleWay|CarWay;
	} else if (xmlStrEqual(type, BAD_CAST "residential")) {
		return FootWay|CycleWay|CarWay;
	} else if (xmlStrEqual(type, BAD_CAST "road")) {
		return FootWay|CycleWay|CarWay;
	} else if (xmlStrEqual(type, BAD_CAST "steps")) {
		return FootWay;
	} else if (xmlStrEqual(type, BAD_CAST "motorway")) {
		return CarWay;
	}


	return CarWay|CycleWay;
}

void OsmParser::parseTagWay(const xmlChar** attrs)
{
	const int k = 1;
	const int v = 3;

	if (xmlStrEqual(attrs[k], BAD_CAST "name")) {
		string name((char*) attrs[v]);
		current_way->name = name;
	} else if (xmlStrEqual(attrs[k], BAD_CAST "highway")) {
		current_way->type = highwayType(attrs[v]);
	} else if (xmlStrEqual(attrs[k], BAD_CAST "foot")) {
		current_way->type |= FootWay;
	} else if (xmlStrEqual(attrs[k], BAD_CAST "bicycle")) {
		current_way->type |= CycleWay;
	} else if (xmlStrEqual(attrs[k], BAD_CAST "railway")) {
		if (xmlStrEqual(attrs[v], BAD_CAST "tram")) {
			current_way->type |= TramWay;
		}
	}
}

void OsmParser::validCurrentWay()
{
	if (0 != current_way->type) {
		vector<Node*>::iterator it;

		for (it = current_way_nodes.begin(); it + 1 != current_way_nodes.end(); ++it) {
			Node* node = *it;
			node->ways[current_way->id] = current_way;

			Node* next_node = *(it + 1);
			next_node->ways[current_way->id] = current_way;

			long distance = Edge::distanceBetween(node, next_node);
			Edge* e = new Edge();
			e->to = next_node;
			e->way = current_way;
			e->distance = distance;
			node->neighbors.insert(e);

			e = new Edge();
			e->to = node;
			e->way = current_way;
			e->distance = distance;
			next_node->neighbors.insert(e);
		}	
	} else {
		ways.erase(current_way->id);
		delete current_way;
	}

	current_way = NULL;
}

void OsmParser::parseNodeWay(const xmlChar** attrs)
{
	int ref = atoi((char*) attrs[1]);

	Node* node = nodes[ref];

	// La node n'a pas été référencée : La voie a une partie en dehors de la zone
	if (NULL == node) {
		return;
	}

	current_way_nodes.push_back(node);
}

void OsmParser::parseWay(const xmlChar** attrs)
{
	Way* way = new Way();
	for (int i = 0; attrs[i] != NULL; i += 2) {
		if (xmlStrEqual(attrs[i], BAD_CAST "id")) {
			way->id = atoi((char*) attrs[i + 1]);
		}
	}

	current_way = way;
	ways[way->id] = way;
	current_way_nodes.clear();
}

void OsmParser::parseNode(const xmlChar** attrs)
{
	Node* node = new Node();

	for (int i = 0; attrs[i] != NULL; i += 2) {
		if (xmlStrEqual(attrs[i], BAD_CAST "id")) {
			node->id = atoi((char*) attrs[i + 1]);
		} else if (xmlStrEqual(attrs[i], BAD_CAST "lat")) {
			node->lat = atof((char*) attrs[i + 1]);
		} else if (xmlStrEqual(attrs[i], BAD_CAST "lon")) {
			node->lon = atof((char*) attrs[i + 1]);
		}
	}

	nodes[node->id] = node;
}

