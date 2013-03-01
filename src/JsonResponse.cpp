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
	if (edges.empty()) {
		return false;
	}

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

	// Points représentant les stations importantes
	for (EdgeGroup group_edge: group_edge_list) {
		if (!group_edge.transport_lines.empty()) {
			Node* first = group_edge.edges.front()->from;
			Node* last = group_edge.edges.back()->to;
			json << "{";
			json << "\"type\":\"Feature\",";
			json << "\"geometry\":{";
			json << "\"type\":\"Point\",";
			json << "\"coordinates\":";
			json << "[" << first->lon << "," << first->lat << "]";
			json << "},";
			json << "\"properties\":{";
			json << "\"type\":\"stop\",";
			json << "\"name\":\"" << first->name << "\"";
			json << "}";
			json << "},";

			json << "{";
			json << "\"type\":\"Feature\",";
			json << "\"geometry\":{";
			json << "\"type\":\"Point\",";
			json << "\"coordinates\":";
			json << "[" << last->lon << "," << last->lat << "]";
			json << "},";
			json << "\"properties\":{";
			json << "\"type\":\"stop\",";
			json << "\"name\":\"" << last->name << "\"";
			json << "}";
			json << "},";
		}
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

	json << "],";
	json << "\"properties\":{";
	json << "\"instructions\":[";

	bool is_first = true; 
	for (EdgeGroup group_edge: group_edge_list) {
		if (!group_edge.transport_lines.empty()) {
			Node* first = group_edge.edges.front()->from;
			Node* last = group_edge.edges.back()->to;
			if (is_first) {
				json << "{";
				is_first = false;
			} else {
				json << ",{";
			}
			json << "\"line\":\"" << group_edge.transport_lines.begin()->second->ref << "\",";
			json << "\"type\":\"" << ((int) group_edge.transport_lines.begin()->second->type == (int) BUS_LINE ? "bus" : "tram") << "\",";
			json << "\"source_stop\":\"" << first->name << "\",";
			json << "\"target_stop\":\"" << last->name << "\"";
			json << "}";
		}
	}
	json << "]";
	json << "}";
	json << "}";

	_response = json.str();

	return true;
}

map<long, TransportLine*> JsonResponse::_commonTransportLines(const map<long, TransportLine*>& l1, const map<long, TransportLine*>& l2) const
{
	map<long, TransportLine*> result;

	for (auto line_entry: l1) {
		if (l2.find(line_entry.first) != l2.end()) {
			result[line_entry.first] = line_entry.second;
		}
	}

	return result;
}

list<EdgeGroup> JsonResponse::_mergeEdgeGroup(list<EdgeGroup> group_edge_list) const
{
	list<EdgeGroup> result;
	map<long, TransportLine*> lines, current_lines;
	EdgeGroup current;

	list<EdgeGroup>::iterator it;

	it = group_edge_list.begin();
	while (it != group_edge_list.end()) {
		current.edges.clear();
		lines = it->transport_lines;
		do {
			current_lines = lines;
			current.edges.insert(current.edges.end(), it->edges.begin(), it->edges.end());
			it++;
			lines = _commonTransportLines(lines, it->transport_lines);
		} while (it != group_edge_list.end() && !lines.empty());

		current.transport_lines = current_lines;

		result.push_back(current);
	}

	return result;
}

list<EdgeGroup> JsonResponse::_getEdgeGroupList(list<Edge*> edges) const
{
	list<EdgeGroup> result;

	EdgeGroup group_edge;
	list<Edge*>::iterator it = edges.begin();

	Edge* e1;
	Edge* e2;
	while (it != edges.end()) {
		group_edge.edges.clear();
		e1 = *it;

		if (e1->from->bus_stop || e1->from->tram_stop) {
			while (it != edges.end()) {
				e2 = *it;
				group_edge.edges.push_back(e2);
				it++;

				if (e2->to->bus_stop || e2->to->tram_stop) {
					break;
				}
			}

			group_edge.transport_lines = _commonTransportLines(e1->way->transport_lines, e2->way->transport_lines);
			result.push_back(group_edge);
		} else {
			while (it != edges.end()) {
				e2 = *it;
				group_edge.edges.push_back(e2);
				it++;

				if (e2->to->bus_stop || e2->to->tram_stop) {
					break;
				}
			}

			group_edge.transport_lines.clear();
			result.push_back(group_edge);
		}
	}

	return _mergeEdgeGroup(result);
}

string JsonResponse::getResponse() const
{
	return _response;
}
