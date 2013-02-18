#ifndef OSM_LOADER_HPP
#define OSM_LOADER_HPP

#include "OsmParser.hpp"
#include "RoutingGraph.hpp"
#include <string>

class OsmLoader
{
	public:
		OsmLoader();
		bool run(std::string osm_filename);

		RoutingGraph& getRoutingGraph();

	private:
		bool _loadTorus(std::string osm_filename);
		bool _saveTorus(std::string osm_filename);

		RoutingGraph _graph;

};

#endif
