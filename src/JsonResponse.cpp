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

	// Une LineString par LineEdges
	list<LineEdges> line_edges_list = _getLineEdges(edges);
	for (LineEdges line_edges: line_edges_list) {
		json << "{";
		json << "\"type\":\"Feature\",";
		json << "\"geometry\":{";
		json << "\"type\":\"LineString\",";
		json << "\"coordinates\":[";

		for (Edge* e: line_edges.edges) {
			json << "[" << e->from->lon << "," << e->from->lat << "],";
		}

		json << "[" << line_edges.edges.back()->to->lon << "," << line_edges.edges.back()->to->lat << "]";
		json << "]";
		json << "},"; 

		if (NULL != line_edges.transport_line) {
			json << "\"properties\":{";
			json << "\"line\":\"" << line_edges.transport_line->ref << "\",";
			json << "\"color\":\"" << line_edges.transport_line->color << "\",";
			json << "\"type\":\"" << ((int) line_edges.transport_line->type == (int) BUS_LINE ? "bus" : "tram") << "\"";
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

TransportLine* JsonResponse::_commonTransportLine(map<long, TransportLine*> l1, map<long, TransportLine*> l2) const
{
	cout << "__" << endl;
	for (auto line_entry: l1) {
		cout << "Ligne " << line_entry.second->ref << endl;
	}
	cout << "##" << endl;
	for (auto line_entry: l2) {
		cout << "Ligne " << line_entry.second->ref << endl;
	}

	for (auto line_entry: l1) {
		if (l2[line_entry.first] != NULL) {
			cout << "==> " << line_entry.second->ref << endl;
			return line_entry.second;
		} else {
			l2.erase(line_entry.first);
		}
	}

	return NULL;
}

list<LineEdges> JsonResponse::_getLineEdges(list<Edge*> edges) const
{
	list<LineEdges> result;

	LineEdges line_edges;
	list<Edge*>::iterator it1 = edges.begin();
	list<Edge*>::iterator it2;

	Edge* e1;
	Edge* e2;
	while (it1 != edges.end()) {
		line_edges.edges.clear();
		line_edges.transport_line = NULL;
		e1 = *it1;

		if (e1->from->bus_stop || e1->from->tram_stop) {
			it2 = it1;
			while (it2 != edges.end()) {
				e2 = *it2;
				line_edges.edges.push_back(e2);
				it2++;

				if (e2->to->bus_stop || e2->to->tram_stop) {
					break;
				}
			}

			line_edges.transport_line = _commonTransportLine(e1->way->transport_lines, e2->way->transport_lines);
			result.push_back(line_edges);
		} else {
			it2 = it1;
			while (it2 != edges.end()) {
				e2 = *it2;
				line_edges.edges.push_back(e2);
				it2++;

				if (e2->to->bus_stop || e2->to->tram_stop) {
					break;
				}
			}

			line_edges.transport_line = NULL;
			result.push_back(line_edges);
		}

		it1 = it2;
	}

	return result;
}

string JsonResponse::getResponse() const
{
	return _response;
}
