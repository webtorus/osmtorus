#ifndef ROUTING_TRACER_HPP
#define ROUTING_TRACER_HPP

#include "OsmLoader.hpp"
#include <vector>

class RoutingTracer
{
	public:
		RoutingTracer();
		bool run(double lat1, double lng1, double lat2, double lng2, short routing_type, OsmLoader& osm_loader);

		std::vector<long> getRoutingNodesIds() const;

	private:
		std::vector<long> _routing_nodes_ids;
};

#endif
