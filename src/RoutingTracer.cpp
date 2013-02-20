#include "include/RoutingTracer.hpp"
#include "include/Edge.hpp"
#include "include/Node.hpp"
#include "include/OsmLoader.hpp"
#include "include/RoutingGraph.hpp"
#include "include/RoutingTracerNode.hpp"
#include "include/WayType.hpp" 
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

RoutingTracer::RoutingTracer()
{

}

bool RoutingTracer::run(double lat1, double lng1, double lat2, double lng2, short authorized_routing_type, OsmLoader& osm_loader)
{
	bool routing_find = false;
	double smallest_time = 0.0;
	std::set<RoutingTracerNode*> routing_tracer_nodes;
	std::set<RoutingTracerNode*> tracer_leaf_nodes;
	
	RoutingGraph& routing_graph = osm_loader.getRoutingGraph();
	RoutingTracerNode* selected_leaf_node = NULL;
	RoutingTracerNode* tracer_new_node = NULL;

	_routing_nodes_ids.clear();

	Node* source_node = getClosestNodeByLatLng(lat1, lng1, routing_graph);

	if(!source_node) {
		return false;
	}

	Node* target_node = getClosestNodeByLatLng(lat2, lng2, routing_graph);

	if(!target_node) {
		return false;
	}

	selected_leaf_node = new RoutingTracerNode(source_node);
	selected_leaf_node->target_node_time = distance(
		source_node->lat,
		source_node->lon,
		target_node->lat,
		target_node->lon) / speedWayType(authorized_routing_type);
	routing_tracer_nodes.insert(selected_leaf_node);
	tracer_leaf_nodes.insert(selected_leaf_node);

	while (!routing_find && !tracer_leaf_nodes.empty()) {
		smallest_time = std::numeric_limits<double>::max();

		for (RoutingTracerNode* tracer_leaf_node: tracer_leaf_nodes) {
			if (tracer_leaf_node->source_node_time + tracer_leaf_node->target_node_time < smallest_time) {
				smallest_time = tracer_leaf_node->source_node_time + tracer_leaf_node->target_node_time;
				selected_leaf_node = tracer_leaf_node;
			}
		}

		tracer_leaf_nodes.erase(selected_leaf_node);

		for (Edge* edge: selected_leaf_node->node->neighbors) {
			if ((edge->way->type & authorized_routing_type) != 0) {
				bool is_placed_node = false;
				double source_node_time = selected_leaf_node->source_node_time
					+ distance(
						selected_leaf_node->node->lat,
						selected_leaf_node->node->lon,
						edge->to->lat,
						edge->to->lon) / speedWayType(edge->way->type & authorized_routing_type);

				for (RoutingTracerNode* routing_tracer_node: routing_tracer_nodes) {
					if(edge->to == routing_tracer_node->node) {
						is_placed_node = true;
					}
				}

				if (is_placed_node) {
					// ?
				} else {
					tracer_new_node = new RoutingTracerNode(edge->to, edge);

					tracer_new_node->parent = selected_leaf_node;
					tracer_new_node->source_node_time = source_node_time;
					tracer_new_node->target_node_time = distance(
						edge->to->lat,
						edge->to->lon,
						target_node->lat,
						target_node->lon) / speedWayType(edge->way->type & authorized_routing_type);
					routing_tracer_nodes.insert(tracer_new_node);
					tracer_leaf_nodes.insert(tracer_new_node);
					// tracer_new_node->_parent->_children.push_back(tracer_new_node);

					if(tracer_new_node->node == target_node) {
						routing_find = true;
					}
				}
			}
		}
	}

	if (routing_find) {
		while (tracer_new_node != NULL) {
			_routing_edges.push_front(tracer_new_node->ingoing_edge);
			tracer_new_node = tracer_new_node->parent;
		}

		_routing_edges.pop_front();
	}

	return true;
}

Node* RoutingTracer::getClosestNodeByLatLng(double lat, double lng, RoutingGraph& routing_graph)
{
	double local_distance = 0.0;
	double shortest_distance = std::numeric_limits<double>::max();
	set<Node*> boundingbox_nodes;
	Node* closest_node = NULL;

	routing_graph.boxes.getBoxesOf(lat, lng, 1, boundingbox_nodes);

	for(Node* node: boundingbox_nodes) {
		local_distance = distance(node->lat, node->lon, lat, lng);

		if(local_distance < shortest_distance) {
			shortest_distance = local_distance;
			closest_node = node;
		}
	}

	return closest_node;
}

double RoutingTracer::distance(double lat1, double lng1, double lat2, double lng2)
{
	double lat_diff = abs(lat1 - lat2);
	double lng_diff = fmod(abs(ffmod(lng1, 360.0) - ffmod(lng2, 360.0)), 180.0);

	return sqrt(lat_diff * lat_diff + lng_diff * lng_diff);
}

double RoutingTracer::ffmod(double numerator, double denominator)
{
	if(0.0 == denominator) {
		return numerator;
	}

	return numerator - denominator * floor(numerator / denominator);
}

double RoutingTracer::speedWayType(short way_type)
{
	if(way_type & CAR_WAY) {
		return 35.0;
	} else if(way_type & TRAM_WAY) {
		return 17.0;
	} else if(way_type & BUS_WAY) {
		return 15.0;
	} else if(way_type & CYCLE_WAY) {
		return 14.0;
	} else if(way_type & FOOT_WAY) {
		return 4.0;
	}

	return std::numeric_limits<double>::max();
}

std::list<Edge*> RoutingTracer::getRoutingEdges() const
{
	return _routing_edges;
}

std::vector<long> RoutingTracer::getRoutingNodesIds() const
{
	return _routing_nodes_ids;
}
