#ifndef ROUTING_TRACER_HPP
#define ROUTING_TRACER_HPP

#include "OsmLoader.hpp"
#include <vector>

class RoutingTracer
{
	public:
		RoutingTracer();
		bool run(double lat1, double lng1, double lat2, double lng2, short authorized_routing_type, OsmLoader& osm_loader);
		bool obtainNearestNodeIdByLatLng(double lat, double lng, RoutingGraph& routing_graph, long& nearest_node_id);
		double distance(double lat1, double lng1, double lat2, double lng2);
		double ffmod(double numerator, double denominator);
		double speedWayType(short way_type);

		std::vector<long> getRoutingNodesIds() const;

	private:
		std::vector<long> _routing_nodes_ids;
};

#endif
