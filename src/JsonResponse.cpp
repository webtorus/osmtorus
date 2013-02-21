#include "include/JsonResponse.hpp"
#include "include/Edge.hpp"
#include "include/Way.hpp"
#include "include/Node.hpp"
#include "include/TransportLine.hpp"
#include <string>
#include <sstream>
#include <set>
#include <iostream>

using namespace std;

JsonResponse::JsonResponse()
{

}

bool JsonResponse::run(double lat1, double lng1, double lat2, double lng2, list<Edge*> edges, const RoutingGraph& routing_graph)
{
	ostringstream json;

	json << "{";
	json << "\"type\":\"FeatureCollection\",";
	json << "\"features\":[";

	// Point de départ
	json << "{";
	json << "\"type\":\"Feature\",";
	json << "\"geometry\":{";
	json << "\"type\":\"Point\",";
	json << "\"coordinates\":";
	json << "[" << lng1 << "," << lat1 << "]";
	json << "},";
	json << "\"properties\":{";
	json << "\"type\":\"source\"";
	json << "}";
	json << "},";

	// Point d'arrivée
	json << "{";
	json << "\"type\":\"Feature\",";
	json << "\"geometry\":{";
	json << "\"type\":\"Point\",";
	json << "\"coordinates\":";
	json << "[" << lng2 << "," << lat2 << "]";
	json << "},";
	json << "\"properties\":{";
	json << "\"type\":\"target\"";
	json << "}";
	json << "},";

	// Tracé entre point de départ réel et 1° arête
	json << "{";
	json << "\"type\":\"Feature\",";
	json << "\"geometry\":{";
	json << "\"type\":\"LineString\",";
	json << "\"coordinates\":[";
	json << "[" << lng1 << "," << lat1 << "],";
	json << "[" << edges.front()->from->lon << "," << edges.front()->from->lat << "]";
	json << "]";
	json << "}";
	json << "},";

	// Une LineString par EdgeGroup
	list<EdgeGroup> group_edge_list = _getEdgeGroupList(edges);
	for (EdgeGroup group_edge: group_edge_list) {
		json << "{";
		json << "\"type\":\"Feature\",";
		json << "\"geometry\":{";
		json << "\"type\":\"LineString\",";
		json << "\"coordinates\":[";

		for (Edge* e: group_edge.edges) {
			json << "[" << e->from->lon << "," << e->from->lat << "],";
		}

		json << "[" << group_edge.edges.back()->to->lon << "," << group_edge.edges.back()->to->lat << "]";
		json << "]";
		json << "},"; 

		if (!group_edge.transport_lines.empty()) {
			TransportLine* transport_line = group_edge.transport_lines.begin()->second;
			json << "\"properties\":{";
			json << "\"line\":\"" << transport_line->ref << "\",";
			json << "\"color\":\"" << transport_line->color << "\",";
			json << "\"type\":\"" << ((int) transport_line->type == (int) BUS_LINE ? "bus" : "tram") << "\"";
			json << "}";
		} else {
			json << "\"properties\":{}";
		}

		json << "},";
	}

	// Tracé entre dernière arête et dernier point réel
	json << "{";
	json << "\"type\":\"Feature\",";
	json << "\"geometry\":{";
	json << "\"type\":\"LineString\",";
	json << "\"coordinates\":[";
	json << "[" << edges.back()->to->lon << "," << edges.back()->to->lat << "],";
	json << "[" << lng2 << "," << lat2 << "]";
	json << "]";
	json << "}";
	json << "}";
	json << "]";
	json << "}";

	_response = json.str();

	return true;
}

map<long, TransportLine*> JsonResponse::_commonTransportLines(map<long, TransportLine*> l1, map<long, TransportLine*> l2) const
{
	map<long, TransportLine*> result;

	for (auto line_entry: l1) {
		if (l2[line_entry.first] != NULL) {
			result[line_entry.first] = line_entry.second;
		} else {
			l2.erase(line_entry.first);
		}
	}

	return result;
}

list<EdgeGroup> JsonResponse::_mergeEdgeGroup(list<EdgeGroup> group_edge_list) const
{
	list<EdgeGroup> result;
	map<long, TransportLine*> lines, current_lines;
	EdgeGroup current;

	list<EdgeGroup>::iterator it1, it2;

	it1 = group_edge_list.begin();
	while (it1 != group_edge_list.end()) {
		current.edges.clear();
		it2 = it1;
		lines = it1->transport_lines;
		do {
			current_lines = lines;
			current.edges.insert(current.edges.end(), it2->edges.begin(), it2->edges.end());
			lines = _commonTransportLines(lines, it2->transport_lines);
			it2++;
		} while (it2 != group_edge_list.end() && !lines.empty());

		current.transport_lines = current_lines;
		result.push_back(current);

		it1 = it2;
	}

	return result;
}

list<EdgeGroup> JsonResponse::_getEdgeGroupList(list<Edge*> edges) const
{
	list<EdgeGroup> result;

	EdgeGroup group_edge;
	list<Edge*>::iterator it1 = edges.begin();
	list<Edge*>::iterator it2;

	Edge* e1;
	Edge* e2;
	while (it1 != edges.end()) {
		group_edge.edges.clear();
		e1 = *it1;

		if (e1->from->bus_stop || e1->from->tram_stop) {
			it2 = it1;
			while (it2 != edges.end()) {
				e2 = *it2;
				group_edge.edges.push_back(e2);
				it2++;

				if (e2->to->bus_stop || e2->to->tram_stop) {
					break;
				}
			}

			group_edge.transport_lines = _commonTransportLines(e1->way->transport_lines, e2->way->transport_lines);
			result.push_back(group_edge);
		} else {
			it2 = it1;
			while (it2 != edges.end()) {
				e2 = *it2;
				group_edge.edges.push_back(e2);
				it2++;

				if (e2->to->bus_stop || e2->to->tram_stop) {
					break;
				}
			}

			group_edge.transport_lines.clear();
			result.push_back(group_edge);
		}

		it1 = it2;
	}

	return _mergeEdgeGroup(result);
}

string JsonResponse::getResponse() const
{
	return _response;
}
