#include "../include/Edge.hpp"
#include "../include/Node.hpp"
#include "../include/RoutingGraph.hpp"
#include "../include/RoutingOsmParser.hpp"
#include "../include/RoutingStorage.hpp"
#include "../include/ParsedNode.hpp"
#include "../include/ParsedRelation.hpp"
#include "../include/ParsedWay.hpp"
#include "../include/TransportLine.hpp"
#include "../include/Way.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << "instructions: ./parse <file.osm>" << endl;
		return -1;
	}

	string file_path(argv[1]);

	cout << "Analyse du fichier OSM" << endl;
	RoutingOsmParser parser;
	try {
		parser.parseFile(file_path);
	} catch (const exception& ex) {
	    cerr << "exception: " << ex.what() << endl;
	}

	cout << "Création du graphe d'itinéraires" << endl;
	RoutingGraph* g = parser.createRoutingGraph();

	cout << g->nodes.size() << endl;
	cout << g->edges.size() << endl;
	cout << g->transport_lines.size() << endl;
	cout << g->ways.size() << endl;

	RoutingStorage storage1(*g);

	// 4 fichiers pour la sauvegarde
	cout << "Enregistrement des données" << endl;

	string nodes_file_path = file_path + ".nodes.torus";
	string lines_file_path = file_path + ".lines.torus";
	string ways_file_path = file_path + ".ways.torus";
	string edges_file_path = file_path + ".edges.torus";
	
	ofstream nodes_file(nodes_file_path.c_str(), ios::out | ios::binary);
	ofstream lines_file(lines_file_path.c_str(), ios::out | ios::binary);
	ofstream ways_file(ways_file_path.c_str(), ios::out | ios::binary);
	ofstream edges_file(edges_file_path.c_str(), ios::out | ios::binary);

	storage1.write(nodes_file, lines_file, ways_file, edges_file);

	nodes_file.close();
	lines_file.close();
	ways_file.close();
	edges_file.close();

	delete g;

	/** TEST LECTURE FICHIERS **/
	cout << "___________ TESTS ___________" << endl;
	ifstream nodes_file_r(nodes_file_path.c_str(), ios::in | ios::binary);
	ifstream lines_file_r(lines_file_path.c_str(), ios::in | ios::binary);
	ifstream ways_file_r(ways_file_path.c_str(), ios::in | ios::binary);
	ifstream edges_file_r(edges_file_path.c_str(), ios::in | ios::binary);

	RoutingGraph graph;
	RoutingStorage storage2(graph);
	storage2.read(nodes_file_r, lines_file_r, ways_file_r, edges_file_r);

	cout << "Taille des données" << endl;
	cout << "Nodes = " << graph.nodes.size() << endl;
	cout << "Edges = " << graph.edges.size() << endl;
	cout << "Transport Lines = " << graph.transport_lines.size() << endl;
	cout << "Ways = " << graph.ways.size() << endl;

	cout << endl << endl; 

	cout << "Départ" << endl;
	Node* first = *(graph.boxes.getBoxOf(43.608566,3.879429)->begin());
	cout << "Node -> " << first->id << " " << first->lat << " " << first->lon << endl;
	map<long, Way*>::iterator it_w;
	for (it_w = first->ways.begin(); it_w != first->ways.end(); it_w++) {
		Way* way = it_w->second;
		cout << "Way -> " << way->id << " " << way->name << endl;

		map<long, TransportLine*>::iterator it_lt;
		for (it_lt = way->transport_lines.begin(); it_lt != way->transport_lines.end(); it_lt++) {
			TransportLine* line = it_lt->second;
			cout << "Line -> " << line->color << " " << line->ref << " " << line->type << endl;
		}
	}

	srand(time(NULL));
	for (int i = 0; i < 1000000; i++) {
		int index = rand() % first->neighbors.size();
		set<Edge*>::iterator it = first->neighbors.begin();
		for (int i = 0; i < index; i++) {
			it++;
		}
		first = (*it)->to;
	}

	cout << "Arrivée" << endl;
	cout << "Node -> " << first->id << " " << first->lat << " " << first->lon << endl;
	for (it_w = first->ways.begin(); it_w != first->ways.end(); it_w++) {
		Way* way = it_w->second;
		cout << "Way -> " << way->id << " " << way->name << endl;

		map<long, TransportLine*>::iterator it_lt;
		for (it_lt = way->transport_lines.begin(); it_lt != way->transport_lines.end(); it_lt++) {
			TransportLine* line = it_lt->second;
			cout << "Line -> " << line->color << " " << line->ref << " " << line->type << endl;
		}
	}

	return 0;
}
