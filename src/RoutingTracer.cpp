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

/*
 * to do
 * algo trouver le noeud le plus proche d'une coordonnee
 *  trouver la boundingbox correspondant a la coordonnee
 *  selectionner tous les boundingbox susceptibles de contenir le noeud recherche
 *  selectionner tous les noeuds des boundingsbox sélectionnees
 *  comparaison distance entre les noeuds(naif, amelioration: possibilite de supprimer des noeuds sans calcul de la distance => plus rapide)
 *  faire l'algo pour les coordonnees de depart et d'arrivee
 * algo a star
 *  selectionner noeud de depart
 *  mettre les noeuds voisins du noeud depart dans une liste (1)
 *  calcul pour chaque noeud le temps = temps_parent + distance(noeud, noeud_parent) / vitesse_typeroute
 *  calcul pour chaque noeud le temps hypothetique pour arrive au noeud arrive = temps + distance(noeud, noeud_arrive) / vitesse_typeroute
 *  selectionner noeud avec le plus faible temps hypothetique
 *  si ce noeud etait deja dans la liste avant cette boucle et que temps_noeud_actuel < temps_noeud_ancien alors remplacer noeud et supprimer la liaison noeud_ancien parent_ancien
 *  refaire l'algo a partir de (1)
 * en dessous de la seconde pour traitement du requete route
 **/

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
	RoutingGraph routing_graph = osm_loader.getRoutingGraph();
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

	std::cout << "source_node: " << routing_graph.nodes[source_node_id]->lat << " " << routing_graph.nodes[source_node_id]->lon << std::endl;
	std::cout << "target_node: " << routing_graph.nodes[target_node_id]->lat << " " << routing_graph.nodes[target_node_id]->lon << std::endl;

	routing_tracer_selected_leaf_node = new RoutingTracerNode(source_node_id);
	routing_tracer_selected_leaf_node->_target_node_time = distance(
		routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lat,
		routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lon,
		routing_graph.nodes[target_node_id]->lat,
		routing_graph.nodes[target_node_id]->lon) / speedWayType(FOOT_WAY);
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
			bool is_leaf_node = false;
			double source_node_time = routing_tracer_selected_leaf_node->_source_node_time
				+ distance(
					routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lat,
					routing_graph.nodes[routing_tracer_selected_leaf_node->_id]->lon,
					edge->to->lat,
					edge->to->lon) / speedWayType(edge->way->type);

			for(RoutingTracerNode* routing_tracer_node: routing_tracer_leaf_nodes) {
				if(edge->to->id == routing_tracer_node->_id) {
					is_leaf_node = true;
				}
			}

			if(is_leaf_node) {

			} else {
				routing_tracer_new_node = new RoutingTracerNode(edge->to->id);

				routing_tracer_new_node->_parent = routing_tracer_selected_leaf_node;
				routing_tracer_new_node->_source_node_time = source_node_time;
				routing_tracer_new_node->_target_node_time = distance(
					routing_graph.nodes[edge->to->id]->lat,
					routing_graph.nodes[edge->to->id]->lon,
					routing_graph.nodes[target_node_id]->lat,
					routing_graph.nodes[target_node_id]->lon) / speedWayType(edge->way->type);
				routing_tracer_nodes.insert(routing_tracer_new_node);
				routing_tracer_leaf_nodes.insert(routing_tracer_new_node);
				routing_tracer_new_node->_parent->_children.push_back(routing_tracer_new_node);

				if(routing_tracer_new_node->_id == target_node_id) {
					routing_find = true;
				}
			}
		}
	}

	if(routing_find) {
		while(routing_tracer_new_node != NULL) {
			_routing_nodes_ids.push_back(routing_tracer_new_node->_id);
			routing_tracer_new_node = routing_tracer_new_node->_parent;
		}
	}

	for(RoutingTracerNode* routing_tracer_node: routing_tracer_nodes) {
		delete routing_tracer_node;
	}

	return false;
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
