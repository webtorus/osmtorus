#include "include/JsonResponse.hpp"
#include "include/OsmLoader.hpp"
#include <string>
#include <vector>

JsonResponse::JsonResponse()
{

}

bool JsonResponse::run(double lat1, double lng1, double lat2, double lng2, std::vector<long> routing_nodes_ids, OsmLoader& osm_loader)
{
	//todo marc => the json in the attribute _response
	return false;
}

std::string JsonResponse::getResponse() const
{
	return _response;
}
