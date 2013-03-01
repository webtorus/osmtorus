#include <fstream>
#include "include/RoutingStorage.hpp"
#include "include/OsmLoader.hpp"
#include "include/RoutingOsmParser.hpp"

using namespace std;

OsmLoader::OsmLoader()
{

}

bool OsmLoader::run(string osm_filename)
{
	ifstream osm_file(osm_filename.c_str());

	// Si les fichiers torus existent, 
	// pas besoin de parser le OSM 
	if (_loadTorus(osm_filename)) {
		return true;
	}

	// Si le fichier osm n'existe pas...
	if (!osm_file.good()) {
		return false;
	}

	osm_file.close();

	// Enregistre en torus les données du fichier OSM
	return _saveTorus(osm_filename);
}

bool OsmLoader::_saveTorus(string osm_filename)
{
	RoutingOsmParser parser;
	try {
		parser.parseFile(osm_filename);
	} catch (const exception& ex) {
		cerr << "exception: " << ex.what() << endl;
	    return false;
	}

	parser.createRoutingGraph(_graph);

	// Enregistrement des torus
	string nodes_file_path = osm_filename + ".nodes.torus";
	string lines_file_path = osm_filename + ".lines.torus";
	string ways_file_path = osm_filename + ".ways.torus";
	string edges_file_path = osm_filename + ".edges.torus";
	
	ofstream nodes_file(nodes_file_path.c_str(), ios::out | ios::binary);
	ofstream lines_file(lines_file_path.c_str(), ios::out | ios::binary);
	ofstream ways_file(ways_file_path.c_str(), ios::out | ios::binary);
	ofstream edges_file(edges_file_path.c_str(), ios::out | ios::binary);

	RoutingStorage storage(_graph);
	storage.write(nodes_file, lines_file, ways_file, edges_file);

	nodes_file.close();
	lines_file.close();
	ways_file.close();
	edges_file.close();

	return true;
}

bool OsmLoader::_loadTorus(string osm_filename)
{
	string nodes_file_path = osm_filename + ".nodes.torus";
	string lines_file_path = osm_filename + ".lines.torus";
	string ways_file_path = osm_filename + ".ways.torus";
	string edges_file_path = osm_filename + ".edges.torus";

	// Ouverture des fichiers en lecture
	ifstream nodes_file(nodes_file_path.c_str(), ios::in | ios::binary);
	ifstream lines_file(lines_file_path.c_str(), ios::in | ios::binary);
	ifstream ways_file(ways_file_path.c_str(), ios::in | ios::binary);
	ifstream edges_file(edges_file_path.c_str(), ios::in | ios::binary);

	if (!nodes_file.good() || !lines_file.good() || !ways_file.good() || !edges_file.good()) {
		return false;
	}

	RoutingStorage storage(_graph);
	storage.read(nodes_file, lines_file, ways_file, edges_file);

	nodes_file.close();
	lines_file.close();
	ways_file.close();
	edges_file.close();

	return true;
}

RoutingGraph& OsmLoader::getRoutingGraph()
{
	return _graph;
}

