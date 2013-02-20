#ifndef JSON_RESPONSE_HPP
#define JSON_RESPONSE_HPP

#include "OsmLoader.hpp"
#include "Edge.hpp"
#include <string>
#include <list>

class JsonResponse
{
	public:
		JsonResponse();
		bool run(double lat1, double lng1, double lat2, double lng2, std::list<Edge*> nodes_ids, OsmLoader& osm_loader);

		std::string getResponse() const;

	private:
		std::string _response;
};

#endif
