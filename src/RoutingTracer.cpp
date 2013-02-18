#include "include/RoutingTracer.hpp"

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
