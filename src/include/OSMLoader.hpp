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

		// OsmParser& getOsmParser();
		RoutingGraph& getRoutingGraph();

	private:
		bool _loadTorus(std::string osm_filename);
		// OsmParser _osm_parser;
		RoutingGraph _graph;

};

#endif
