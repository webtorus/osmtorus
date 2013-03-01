#ifndef JSON_RESPONSE_HPP
#define JSON_RESPONSE_HPP

#include "RoutingGraph.hpp"
#include "Edge.hpp"
#include "EdgeGroup.hpp"
#include <string>
#include <list>

class JsonResponse
{
	public:
		JsonResponse();
		bool run(double lat1, double lng1, double lat2, double lng2, std::list<Edge*> nodes_ids, const RoutingGraph& osm_loader);

		std::string getResponse() const;

	private:
		std::string _response;

		std::list<EdgeGroup> _getEdgeGroupList(const std::list<Edge*> edges) const;
		map<long, TransportLine*> _commonTransportLines(std::map<long, TransportLine*> l1, std::map<long, TransportLine*> l2) const;
		list<EdgeGroup> _mergeEdgeGroup(list<EdgeGroup> group_edge_list) const;
};

#endif
