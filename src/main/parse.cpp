#include <iostream>
#include <fstream>
#include <map>
#include "../storage/RoutingStorage.hpp"
#include "../parser/OsmParser.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << "instructions: ./parse <file.osm>" << endl;
		return -1;
	}

	string file_path(argv[1]);

	cout << "Analyse du fichier OSM" << endl;
	OsmParser parser;
	try {
		parser.parseFile(file_path);
	} catch (const exception& ex) {
	    cerr << "exception: " << ex.what() << endl;
	}

	map<long, Node*> nodes;
	map<long, Way*> ways;
	
	nodes = parser.getNodes();
	ways = parser.getWays();

	RoutingStorage storage(nodes, ways);

	// 3 fichiers pour la sauvegarde
	cout << "Enregistrement des données" << endl;

	string nodes_file_path = file_path + ".nodes.torus";
	string ways_file_path = file_path + ".ways.torus";
	string edges_file_path = file_path + ".edges.torus";
	ofstream nodes_file(nodes_file_path.c_str(), ios::out | ios::binary);
	ofstream ways_file(ways_file_path.c_str(), ios::out | ios::binary);
	ofstream edges_file(edges_file_path.c_str(), ios::out | ios::binary);

	storage.write(nodes_file, ways_file, edges_file);

	nodes_file.close();
	ways_file.close();
	edges_file.close();

	/** TEST LECTURE FICHIERS **/
	ifstream nodes_file_r(nodes_file_path.c_str(), ios::in | ios::binary);
	ifstream ways_file_r(ways_file_path.c_str(), ios::in | ios::binary);
	ifstream edges_file_r(edges_file_path.c_str(), ios::in | ios::binary);

	// RoutingStorage storage;
	storage.read(nodes_file_r, ways_file_r, edges_file_r);

	nodes = storage.getNodes();
	ways = storage.getWays();

	cout << nodes.size() << endl;
	cout << ways.size() << endl;

	map<long, Node*>::iterator it = nodes.begin();
	cout << "TESTS" << endl;
	cout << it->second->id << endl;
	cout << it->second->lat << endl;
	cout << it->second->lon << endl;
	it++;
	cout << it->second->id << endl;
	cout << it->second->lat << endl;
	cout << it->second->lon << endl;
	cout << ways.begin()->second->id << endl;
	// cout << (nodes.end() - 1)->second->id << endl;

	// for (it_n = nodes.begin(); it_n != nodes.end(); it_n++) {
	// 	Node* node = it_n->second;
		
	// 	cout << node->id << endl;
	// }

	return 0;
}
