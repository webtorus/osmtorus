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

	if (!osm_file.good()) {
		return false;
	}

	// Si les fichiers torus existent, 
	// pas besoin de parser le OSM 
	if (_loadTorus(osm_filename)) {
		return true;
	}

	RoutingOsmParser parser;
	try {
		parser.parseFile(osm_filename);
	} catch (const exception& ex) {
	    return false;
	}

	parser.createRoutingGraph(_graph);

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

	return true;
}

RoutingGraph& OsmLoader::getRoutingGraph()
{
	return _graph;
}

// OsmParser& OsmLoader::getOsmParser()
// {
// 	return _osm_parser;
// }
