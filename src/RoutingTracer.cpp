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
	bool obtainNearestNodeIdByLatLng_return = false;
	long source_node_id = 0;
	long target_node_id = 0;
	double smallest_time = 0.0;
	std::set<RoutingTracerNode*> routing_tracer_nodes;
	std::set<RoutingTracerNode*> routing_tracer_leaf_nodes;
	RoutingGraph& routing_graph = osm_loader.getRoutingGraph();
	RoutingTracerNode* routing_tracer_selected_leaf_node = NULL;
	RoutingTracerNode* routing_tracer_new_node = NULL;

	_routing_nodes_ids.clear();

	obtainNearestNodeIdByLatLng_return = obtainNearestNodeIdByLatLng(lat1, lng1, routing_graph, source_node_id);

	if(!obtainNearestNodeIdByLatLng_return) {
		return false;
	}

	obtainNearestNodeIdByLatLng_return = obtainNearestNodeIdByLatLng(lat2, lng2, routing_graph, target_node_id);

	if(!obtainNearestNodeIdByLatLng_return) {
		return false;
	}

	routing_tracer_selected_leaf_node = new RoutingTracerNode(source_node_id);
	routing_tracer_selected_leaf_node->_target_node_time = distance(
		routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lat,
		routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lon,
		routing_graph.nodes[target_node_id]->lat,
		routing_graph.nodes[target_node_id]->lon) / speedWayType(authorized_routing_type);
	routing_tracer_nodes.insert(routing_tracer_selected_leaf_node);
	routing_tracer_leaf_nodes.insert(routing_tracer_selected_leaf_node);

	while(!routing_find && routing_tracer_leaf_nodes.size() > 0) {
		smallest_time = std::numeric_limits<double>::max();

		for(RoutingTracerNode* routing_tracer_leaf_node: routing_tracer_leaf_nodes) {
			if(routing_tracer_leaf_node->_source_node_time + routing_tracer_leaf_node->_target_node_time < smallest_time) {
				smallest_time = routing_tracer_leaf_node->_source_node_time + routing_tracer_leaf_node->_target_node_time;
				routing_tracer_selected_leaf_node = routing_tracer_leaf_node;
			}
		}

		routing_tracer_leaf_nodes.erase(routing_tracer_selected_leaf_node);

		for(Edge* edge: routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->neighbors) {
			if((edge->way->type & authorized_routing_type) != 0) {
				bool is_placed_node = false;
				double source_node_time = routing_tracer_selected_leaf_node->_source_node_time
					+ distance(
						routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lat,
						routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lon,
						edge->to->lat,
						edge->to->lon) / speedWayType(edge->way->type & authorized_routing_type);

				for(RoutingTracerNode* routing_tracer_node: routing_tracer_nodes) {
					if(edge->to->id == routing_tracer_node->_id) {
						is_placed_node = true;
					}
				}

				if(is_placed_node) {

				} else {
					routing_tracer_new_node = new RoutingTracerNode(edge->to->id);

					routing_tracer_new_node->_parent = routing_tracer_selected_leaf_node;
					routing_tracer_new_node->_source_node_time = source_node_time;
					routing_tracer_new_node->_target_node_time = distance(
						routing_graph.nodes[edge->to->id]->lat,
						routing_graph.nodes[edge->to->id]->lon,
						routing_graph.nodes[target_node_id]->lat,
						routing_graph.nodes[target_node_id]->lon) / speedWayType(edge->way->type & authorized_routing_type);
					routing_tracer_nodes.insert(routing_tracer_new_node);
					routing_tracer_leaf_nodes.insert(routing_tracer_new_node);
					routing_tracer_new_node->_parent->_children.push_back(routing_tracer_new_node);

					if(routing_tracer_new_node->_id == target_node_id) {
						routing_find = true;
					}
				}
			}
		}
	}

	if(routing_find) {
		std::vector<long> routing_nodes_ids_tmp;

		while(routing_tracer_new_node != NULL) {
			routing_nodes_ids_tmp.push_back(routing_tracer_new_node->_id);
			routing_tracer_new_node = routing_tracer_new_node->_parent;
		}
		
		while(routing_nodes_ids_tmp.size() > 0) {
			_routing_nodes_ids.push_back(routing_nodes_ids_tmp.back());
			routing_nodes_ids_tmp.pop_back();
		}
	}

	for(RoutingTracerNode* routing_tracer_node: routing_tracer_nodes) {
		delete routing_tracer_node;
	}

	return true;
}

bool RoutingTracer::obtainNearestNodeIdByLatLng(double lat, double lng, RoutingGraph& routing_graph, long& nearest_node_id)
{
	double local_distance = 0.0;
	double shortest_distance = std::numeric_limits<double>::max();
	set<Node*> boundingbox_nodes;

	routing_graph.boxes.getBoxesOf(lat, lng, 1, boundingbox_nodes);

	if(boundingbox_nodes.size() == 0) {
		return false;
	}

	for(Node* node: boundingbox_nodes) {
		local_distance = distance(node->lat, node->lon, lat, lng);

		if(local_distance < shortest_distance) {
			shortest_distance = local_distance;
			nearest_node_id = node->id;
		}
	}

	return true;
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

std::vector<long> RoutingTracer::getRoutingNodesIds() const
{
	return _routing_nodes_ids;
}
