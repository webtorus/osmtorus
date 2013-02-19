#ifndef JSON_RESPONSE_HPP
#define JSON_RESPONSE_HPP

#include "OsmLoader.hpp"
#include <string>
#include <vector>

class JsonResponse
{
	public:
		JsonResponse();
		bool run(double lat1, double lng1, double lat2, double lng2, std::vector<long> nodes_ids, OsmLoader& osm_loader);

		std::string getResponse() const;

	private:
		std::string _response;
};

#endif
