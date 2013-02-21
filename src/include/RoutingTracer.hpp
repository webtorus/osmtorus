#ifndef ROUTING_TRACER_HPP
#define ROUTING_TRACER_HPP

#include "RoutingGraph.hpp"
#include <vector>
#include <list>

class RoutingTracer
{
	public:
		RoutingTracer();
		bool run(double lat1, double lng1, double lat2, double lng2, short authorized_routing_type, const RoutingGraph& routing_graph);
		Node* getClosestNodeByLatLng(double lat, double lng, const RoutingGraph& routing_graph);
		double distance(double lat1, double lng1, double lat2, double lng2);
		double ffmod(double numerator, double denominator);
		double speedWayType(short way_type);

		// std::vector<long> getRoutingNodesIds() const;
		std::list<Edge*> getRoutingEdges() const;

	private:
		// std::vector<long> _routing_nodes_ids;
		std::list<Edge*> _routing_edges;
};

#endif
