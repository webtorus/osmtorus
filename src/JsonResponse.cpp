#include "include/JsonResponse.hpp"
#include "include/Edge.hpp"
#include "include/Node.hpp"
#include <string>
#include <sstream>

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

	// coordonnées
	json << "{";
	json << "\"type\":\"Feature\",";
	json << "\"geometry\":{";
	json << "\"type\":\"LineString\",";
	json << "\"coordinates\":[";
	json << "[" << lng1 << "," << lat1 << "],";
	for (Edge* e: edges) {
		Node* node = e->from;
		json << "[" << node->lon << "," << node->lat << "],";
	}
	json << "[" << lng2 << "," << lat2 << "]";
	json << "]";
	json << "},";
	json << "\"properties\": {}";
	json << "}";
	json << "]";
	json << "}";

	_response = json.str();

	return true;
}

std::string JsonResponse::getResponse() const
{
	return _response;
}
