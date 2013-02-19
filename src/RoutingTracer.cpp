#include "include/RoutingTracer.hpp"
#include "include/Node.hpp"
#include "include/OsmLoader.hpp"
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

bool RoutingTracer::run(double lat1, double lng1, double lat2, double lng2, short routing_type, OsmLoader& osm_loader)
{
	long started_id = 0;
	long arrived_id = 0;
	double shortest_distance = 0;
	set<Node*> boundingbox_nodes;

	_routing_nodes_ids.clear();

	boundingbox_nodes.clear();
	osm_loader.getRoutingGraph().boxes.getBoxesOf(lat1, lng1, 1, boundingbox_nodes);
	if(boundingbox_nodes.size() == 0) {
		return false;
	}
	shortest_distance = std::numeric_limits<int>::max();
	for(Node* node: boundingbox_nodes) {
		double local_distance = sqrt((node->lat - lat1) * (node->lat - lat1) + (node->lon - lng1) * (node->lon - lng1));

		if(local_distance < shortest_distance) {
			shortest_distance = local_distance;
			started_id = node->id;
		}
	}

	boundingbox_nodes.clear();
	osm_loader.getRoutingGraph().boxes.getBoxesOf(lat2, lng2, 1, boundingbox_nodes);
	if(boundingbox_nodes.size() == 0) {
		return false;
	}
	shortest_distance = std::numeric_limits<int>::max();
	for(Node* node: boundingbox_nodes) {
		double local_distance = sqrt((node->lat - lat2) * (node->lat - lat2) + (node->lon - lng2) * (node->lon - lng2));

		if(local_distance < shortest_distance) {
			shortest_distance = local_distance;
			arrived_id = node->id;
		}
	}

	std::cout << "started_node: " << osm_loader.getRoutingGraph().nodes[started_id]->lat << " " << osm_loader.getRoutingGraph().nodes[started_id]->lon << std::endl;
	std::cout << "arrived_node: " << osm_loader.getRoutingGraph().nodes[arrived_id]->lat << " " << osm_loader.getRoutingGraph().nodes[arrived_id]->lon << std::endl;

	return false;
}

std::vector<long> RoutingTracer::getRoutingNodesIds() const
{
	return _routing_nodes_ids;
}
