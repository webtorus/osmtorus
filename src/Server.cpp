#include "include/Server.hpp"
#include "include/ConfLoader.hpp"
#include "include/HttpRequesterReaderServer.hpp"
#include "include/HttpRequesterWriterServer.hpp"
#include "include/OSMLoader.hpp"
#include "include/SocketTCP.hpp"
#include "include/Thread.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

/*
 * to do
 *
 * parser fichier conf en entier
 * integrer le parser osm dans loadosm
 * boundingbox sur les noeuds
 * valider requete route
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

void Server::run(int argc, char* argv[])
{
	if(argc == 0 || (argc == 1 && std::string(argv[0]) == "-d")) {
		bool conf_loader_return = false;

		if(argc == 0) {
			conf_loader_return = _conf_loader.run(false);
		} else {
			conf_loader_return = _conf_loader.run(true);
		}

		if(conf_loader_return) {
			std::set<short int> ports = _conf_loader.getPorts();

			if(_conf_loader.getOsmFile() != "") {
				bool osm_loader_return = false;

				osm_loader_return = _osm_loader.run(_conf_loader.getOsmFile());

				if(osm_loader_return) {
					for(short int port: ports) {
						Thread listener_thread;

						listener_mutex.lock();
						listener_thread.create(Server::listener, (void*)&port);
						listener_cond.wait(listener_mutex);
						listener_mutex.unlock();
					}
				}
			}
		} else {
			std::cerr << "Invalid Conf File" << std::endl;
		}
	}

	Thread::exit();
}

void* Server::listener(void* arg)
{
	short int port = 0;
	SocketTCP listener_socket;
	SocketTCP requester_socket;
	Thread requester_thread;

	listener_mutex.lock();
	port = *((short int*)arg);
	listener_cond.signal();
	listener_mutex.unlock();
	listener_socket.binding(port);
	listener_socket.listening(10);

	while(true) {
		bool client_ip_authorized = false;
		std::string client_ip;

		requester_mutex.lock();
		listener_socket.accepting(requester_socket);

		client_ip = requester_socket.getClientIp();

		for(std::string next_ip: _conf_loader.getAuthorizeds()) {
			if(next_ip == client_ip) {
				client_ip_authorized = true;
				break;
			}
		}

		if(client_ip_authorized || _conf_loader.allAuthorized()) {
			requester_thread.create(Server::requester, (void*)&requester_socket);
		}
		
		client_ip_authorized = false;

		requester_cond.wait(requester_mutex);
		requester_mutex.unlock();
	}

	listener_socket.closing();
	Thread::exit();

	return EXIT_SUCCESS;
}

void* Server::requester(void* arg)
{
	HttpRequesterReaderServer http_requester_reader_server;
	HttpRequesterWriterServer http_requester_writer_server;
	SocketTCP requester_socket;

	requester_mutex.lock();
	requester_socket = *((SocketTCP*)arg);
	requester_cond.signal();
	requester_mutex.unlock();
	if(http_requester_reader_server.run(requester_socket)) {
		

		http_requester_writer_server.run();//probable parametre
	} else {
		http_requester_writer_server.run();//probable parametre
	}
	requester_socket.closing();

	Thread::exit();

	return EXIT_SUCCESS;
}

Cond Server::listener_cond;

Cond Server::requester_cond;

Mutex Server::listener_mutex;

Mutex Server::requester_mutex;

ConfLoader Server::_conf_loader;

OSMLoader Server::_osm_loader;
