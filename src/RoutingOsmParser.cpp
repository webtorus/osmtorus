#include "include/RoutingOsmParser.hpp"
#include "include/Edge.hpp"
#include "include/Node.hpp"
#include "include/OsmParser.hpp"
#include "include/ParsedNode.hpp"
#include "include/ParsedRelation.hpp"
#include "include/ParsedWay.hpp"
#include "include/RoutingGraph.hpp"
#include "include/TransportLine.hpp"
#include "include/UniqueIdentifier.hpp"
#include "include/Way.hpp"
#include <libxml/parser.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>

#define K 1
#define V 3

using namespace std;

RoutingOsmParser::RoutingOsmParser(): OsmParser()
{

}

RoutingOsmParser::~RoutingOsmParser()
{

}

void RoutingOsmParser::createRoutingGraph(RoutingGraph& graph)
{
	graph.boxes.min_x = boxes.min_x;
	graph.boxes.min_y = boxes.min_y;
	graph.boxes.max_x = boxes.max_x;
	graph.boxes.max_y = boxes.max_y;

	// création des noeuds
	map<long, ParsedNode*>::iterator it_n;
	for (it_n = nodes.begin(); it_n != nodes.end(); it_n++) {
		ParsedNode* pn = it_n->second;
		Node* node = new Node();
		node->id = pn->id;
		node->lat = pn->lat;
		node->lon = pn->lon;
		node->bus_stop = pn->bus_stop;
		node->tram_stop = pn->tram_stop;

		graph.nodes[node->id] = node;
		graph.boxes.getBoxOf(node->lat, node->lon)->insert(node);
	}

	// création des arêtes
	map<long, ParsedWay*>::iterator it_w;
	for (it_w = ways.begin(); it_w != ways.end(); it_w++) {
		ParsedWay* pw = it_w->second;
		Way* way = new Way();
		way->id = pw->id;
		way->type = pw->type;
		way->name = pw->name;
		graph.ways[way->id] = way;

		vector<ParsedNode*>::iterator it_p;
		for (it_p = pw->path.begin(); it_p + 1 != pw->path.end(); it_p++) {
			ParsedNode* pn = *it_p;
			ParsedNode* pnn = *(it_p + 1);

			unsigned int distance = distanceBetween(pn, pnn);

			Node* node = graph.nodes[pn->id];
			Node* next_node = graph.nodes[pnn->id];
			
			node->ways[way->id] = way;
			next_node->ways[way->id] = way;

			Edge* e = new Edge();
			e->from = node;
			e->to = next_node;
			e->way = way;
			e->length = distance;
			node->neighbors.insert(e);
			graph.edges.insert(e);

			e = new Edge();
			e->from = next_node;
			e->to = node;
			e->way = way;
			e->length = distance;
			next_node->neighbors.insert(e);
			graph.edges.insert(e);
		}
	}

	// ajout des informations de transports
	map<long, ParsedRelation*>::iterator it_r;
	for (it_r = relations.begin(); it_r != relations.end(); it_r++) {
		ParsedRelation* pr = it_r->second;
		TransportLine *line = new TransportLine();
		line->id = UniqueIdentifier::next();
		line->ref =  pr->ref;
		line->color = pr->color;
		if (pr->type == TRAM) {
			line->type = TRAM_LINE;
		} else if (pr->type == BUS) {
			line->type = BUS_LINE;
		}

		map<long, ParsedWay*>::iterator it_rw;
		for (it_rw = pr->ways.begin(); it_rw != pr->ways.end(); it_rw++) {
			ParsedWay* p_way = it_rw->second;
			Way* way = graph.ways[p_way->id];
			way->transport_lines[line->id] = line;
		}

		graph.transport_lines[line->id] = line;
	}
}

void RoutingOsmParser::finish()
{
	cout << nodes.size() << endl;
	cout << relations.size() << endl;
	cout << ways.size() << endl;
	deleteUselessNodes();
	linkBusStopsToBusNetwork();
	linkStopsToNetwork();
}

void RoutingOsmParser::parseTag(const xmlChar** attrs)
{
	OsmParser::parseTag(attrs);
}

void RoutingOsmParser::parseNode(const xmlChar** attrs)
{
	OsmParser::parseNode(attrs);
}

void RoutingOsmParser::parseWay(const xmlChar** attrs)
{
	OsmParser::parseWay(attrs);
}

void RoutingOsmParser::parseRelation(const xmlChar** attrs)
{
	OsmParser::parseRelation(attrs);
}

void RoutingOsmParser::parseTagNode(const xmlChar** attrs)
{
	if (xmlStrEqual(attrs[K], BAD_CAST "highway") && xmlStrEqual(attrs[V], BAD_CAST "bus_stop")) {
		current_node->bus_stop = true;
	} else if (xmlStrEqual(attrs[K], BAD_CAST "railway") && xmlStrEqual(attrs[V], BAD_CAST "tram_stop")) {
		current_node->tram_stop = true;
	} else if (xmlStrEqual(attrs[K], BAD_CAST "name")) {
		string name((char*) attrs[V]);
		current_node->name = name;
	}
}

void RoutingOsmParser::validCurrentNode()
{
	nodes[current_node->id] = current_node;

	OsmParser::validCurrentNode();
}

short RoutingOsmParser::highwayType(const xmlChar* type)
{
	if (xmlStrEqual(type, BAD_CAST "path")) {
		return FOOT_WAY | CYCLE_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "pedestrian")) {
		return FOOT_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "footway")) {
		return FOOT_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "cycleway")) {
		return CYCLE_WAY | FOOT_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "living_street")) {
		return FOOT_WAY | CYCLE_WAY | CAR_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "residential")) {
		return FOOT_WAY | CYCLE_WAY | CAR_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "road")) {
		return FOOT_WAY | CYCLE_WAY | CAR_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "steps")) {
		return FOOT_WAY;
	} else if (xmlStrEqual(type, BAD_CAST "motorway")) {
		return CAR_WAY;
	}

	return CAR_WAY | CYCLE_WAY | FOOT_WAY;
}

void RoutingOsmParser::parseNodeWay(const xmlChar** attrs)
{
	const int ref = 1;

	long id = atol((char*) attrs[ref]);

	ParsedNode* node = nodes[id];

	// La node n'a pas été référencée : La voie a une partie en dehors de la zone
	if (NULL == node) {
		nodes.erase(id);
		return;
	}

	current_way->path.push_back(node);
}

void RoutingOsmParser::parseTagWay(const xmlChar** attrs)
{
	if (xmlStrEqual(attrs[K], BAD_CAST "name")) {
		string name((char*) attrs[V]);
		current_way->name = name;
	} else if (xmlStrEqual(attrs[K], BAD_CAST "highway")) {
		current_way->type = highwayType(attrs[V]);
	} else if (xmlStrEqual(attrs[K], BAD_CAST "foot")) {
		current_way->type |= FOOT_WAY;
	} else if (xmlStrEqual(attrs[K], BAD_CAST "bicycle")) {
		current_way->type |= CYCLE_WAY;
	} else if (xmlStrEqual(attrs[K], BAD_CAST "railway")) {
		if (xmlStrEqual(attrs[V], BAD_CAST "tram")) {
			current_way->type |= TRAM_WAY;
		}
	}
}

void RoutingOsmParser::validCurrentWay()
{
	// Enregistrement des Way de types highway ou railway
	if (current_way->type != 0) {
		vector<ParsedNode*>::iterator it;
		for (it = current_way->path.begin(); it != current_way->path.end(); it++) {
			ParsedNode* node = *it;
			node->ways[current_way->id] = current_way; 
		}
		
		ways[current_way->id] = current_way;
	}

	OsmParser::validCurrentWay();
}

short RoutingOsmParser::relationRouteType(const xmlChar* type)
{
	if (xmlStrEqual(type, BAD_CAST "bus")) {
		return BUS;
	} else if (xmlStrEqual(type, BAD_CAST "tram")) {
		return TRAM;
	}

	return OTHER;
}

void RoutingOsmParser::parseMemberRelation(const xmlChar** attrs)
{
	const int type = 1;
	const int ref = 3;

	long id = atol((char*) attrs[ref]);
	
	if (xmlStrEqual(attrs[type], BAD_CAST "node")) {
		ParsedNode* node = nodes[id];
		if (NULL != node) {
			current_relation->nodes[node->id] = node;
		} else {
			nodes.erase(id);
		}
	} else if (xmlStrEqual(attrs[type], BAD_CAST "way")) {
		ParsedWay* way = ways[id];
		if (NULL != way) {
			current_relation->ways[way->id] = way;
		} else {
			ways.erase(id);
		}
	}
}

void RoutingOsmParser::parseTagRelation(const xmlChar** attrs)
{
	if (xmlStrEqual(attrs[K], BAD_CAST "route")) {
		current_relation->type = relationRouteType(attrs[V]);
	} else if (xmlStrEqual(attrs[K], BAD_CAST "ref")) {
		string ref((char*) attrs[V]);
		current_relation->ref = ref;
	} else if (xmlStrEqual(attrs[K], BAD_CAST "color")) {
		string color((char*) attrs[V]);
		if (color[0] != '#') {
			color = '#' + color;
		}
		current_relation->color = color;
	}
}

void RoutingOsmParser::validCurrentRelation()
{
	// Enregistrement des relations de type BUS ou TRAM
	if (current_relation->type != OTHER) {
		map<long, ParsedNode*>::iterator it_n;
		for (it_n = current_relation->nodes.begin(); it_n != current_relation->nodes.end(); it_n++) {
			ParsedNode* node = it_n->second;
			node->relations[current_relation->id] = current_relation;
		}

		map<long, ParsedWay*>::iterator it_w;
		for (it_w = current_relation->ways.begin(); it_w != current_relation->ways.end(); it_w++) {
			ParsedWay* way = it_w->second;
			way->relations[current_relation->id] = current_relation;

			if (current_relation->type == TRAM) {
				way->type |= TRAM_WAY;
			} else if (current_relation->type == BUS) {
				way->type |= BUS_WAY;
			}
		}

		relations[current_relation->id] = current_relation;
	}

	OsmParser::validCurrentRelation();
}

void RoutingOsmParser::deleteUselessNodes()
{
	// Suppression des nodes inutiles
	map<long, ParsedNode*>::iterator it = nodes.begin();

	cout << "Suppression de nodes inutiles :" << endl;
	cout << "AVANT => " << nodes.size() << endl;
	while (it != nodes.end()) {
		ParsedNode* node = it->second;
		if (NULL == node || (node->ways.empty() && node->relations.empty())) {
			nodes.erase(it++);
			delete node;
		} else {
			boxes.getBoxOf(node->lat, node->lon)->insert(node);
			it++;
		}
	}
	cout << "APRES => " << nodes.size() << endl;
}

double RoutingOsmParser::distanceFromLine(const ParsedNode* point, const ParsedNode* n1, const ParsedNode* n2, ParsedNode* result)
{
	double l2 = (n1->lat - n2->lat) * (n1->lat - n2->lat) + (n1->lon - n2->lon) * (n1->lon - n2->lon);
	if (l2 == 0) {
		result->lat = n1->lat;
		result->lon = n1->lon;
	} else {
		double t = ((point->lat - n1->lat) * (n2->lat - n1->lat) + (point->lon - n1->lon) * (n2->lon - n1->lon)) / l2;
		if (t < 0) {
			result->lat = n1->lat;
			result->lon = n1->lon;
		} else if (t > 1) {
			result->lat = n2->lat;
			result->lon = n2->lon;
		} else {
			result->lat = n1->lat + t * (n2->lat - n1->lat);
			result->lon = n1->lon + t * (n2->lon - n1->lon);
		}
	}
	
	double distance = (result->lat - point->lat) * (result->lat - point->lat) + (result->lon - point->lon) * (result->lon - point->lon);
 
	return distance;
}

unsigned int RoutingOsmParser::distanceBetween(const ParsedNode* n1, const ParsedNode* n2)
{
	double rad = 0.017453292519943295769236907684886;
	double earth_radius = 6372797.560856;

	double latitude_arc  = (n1->lat - n2->lat) * rad;
    double longitude_arc = (n1->lon - n2->lon) * rad;
    double latitude_h = sin(latitude_arc * 0.5);
    latitude_h *= latitude_h;
    double lontitude_h = sin(longitude_arc * 0.5);
    lontitude_h *= lontitude_h;
    double tmp = cos(n1->lat * rad) * cos(n2->lat * rad);

    return ceil(2.0 * asin(sqrt(latitude_h + tmp * lontitude_h)) * earth_radius);
}

void RoutingOsmParser::linkBusStop(ParsedNode* bus_stop, ParsedRelation* relation)
{
	ParsedNode current, best;
	int pos_result = 0;
	ParsedWay* closest_way = 0;
	double min_distance = numeric_limits<double>::max();
	double distance;

	map<long, ParsedWay*>::iterator it_w;
	for (it_w = relation->ways.begin(); it_w != relation->ways.end(); it_w++) {
		ParsedWay* way = it_w->second;

		for (unsigned int i = 0; i + 1 < way->path.size(); i++) {
			ParsedNode* node = way->path[i];
			ParsedNode* next_node = way->path[i + 1];

			distance = distanceFromLine(bus_stop, node, next_node, &current);
			
			if (distance < min_distance) {
				min_distance = distance;
				best.lat = current.lat;
				best.lon = current.lon;
				pos_result = i + 1;
				closest_way = way;
			}
		}
	}

	bus_stop->lat = best.lat;
	bus_stop->lon = best.lon;

	bus_stop->ways[closest_way->id] = closest_way;
	closest_way->path.insert(closest_way->path.begin() + pos_result, bus_stop);
}

void RoutingOsmParser::linkBusStopsToBusNetwork()
{
	cout << "Liaison des stations de bus au réseau des bus" << endl;

	map<long, ParsedRelation*>::iterator it_r;
	for (it_r = relations.begin(); it_r != relations.end(); it_r++) {
		ParsedRelation* relation = it_r->second;

		map<long, ParsedNode*>::iterator it_n;
		for (it_n = relation->nodes.begin(); it_n != relation->nodes.end(); it_n++) {
			ParsedNode* node = it_n->second;
			if (node->ways.empty()) {
				linkBusStop(node, relation);
			}
		}
	}
}

void RoutingOsmParser::linkStop(ParsedNode* stop)
{
	set<ParsedNode*> box;
	boxes.getBoxesOf(stop->lat, stop->lon, 1, box);

	set<ParsedNode*>::iterator it;
	for (it = box.begin(); it != box.end(); it++) {
		ParsedNode* node = *it;

		if (stop->id == node->id) {
			continue;
		}

		unsigned int distance = distanceBetween(stop, node);
		if (distance < 25) {
			ParsedWay* new_way = new ParsedWay();
			new_way->id = UniqueIdentifier::next();
			new_way->type = CYCLE_WAY | FOOT_WAY;
			new_way->path.push_back(stop);
			new_way->path.push_back(node);
			stop->ways[new_way->id] = new_way;
			node->ways[new_way->id] = new_way;

			ways[new_way->id] = new_way;
		}
	}
}

void RoutingOsmParser::linkStopsToNetwork()
{
	cout << "Liaison des stations des transports en commun au réseau global" << endl;
	cout << "AVANT : " << ways.size() << endl;
	map<long, ParsedRelation*>::iterator it_r;
	for (it_r = relations.begin(); it_r != relations.end(); it_r++) {
		ParsedRelation* relation = it_r->second;

		map<long, ParsedNode*>::iterator it_n;
		for (it_n = relation->nodes.begin(); it_n != relation->nodes.end(); it_n++) {
			ParsedNode* node = it_n->second;
			if ((node->bus_stop || node->tram_stop) && node->ways.size() == 1) {
				linkStop(node);
			}
		}
	}

	cout << "APRES : " << ways.size() << endl;
}
