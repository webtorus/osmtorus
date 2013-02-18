#ifndef OSM_LOADER_HPP
#define OSM_LOADER_HPP

#include "OsmParser.hpp"
#include <string>

class OSMLoader
{
	public:
		OSMLoader();
		bool run(std::string osm_filename);

		OsmParser& getOsmParser();

	private:
		OsmParser _osm_parser;
};

#endif
