#include "include/ConfLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

ConfLoader::ConfLoader(): _all_authorized(false)
{

}

bool ConfLoader::run(bool is_daemon)
{
	std::ifstream conf_file;

	if(is_daemon) {
		conf_file.open("/etc/osmtorus/osmtorus.conf");
	} else {
		conf_file.open("osmtorus.conf");
	}

	if(conf_file.good()) {
		std::string line;
		
		while(std::getline(conf_file, line)) {
			std::istringstream line_split(line);
			std::string tag;

			if(line_split) {
				line_split >> tag;

				if(tag == "Listen") {
					short int port = -1;

					line_split >> port;

					if(!line_split.fail()) {
						_ports.push_back(port);
					} else {
						return false;
					}
				} else if(tag == "Authorized") {
					std::string authorized;

					line_split >> authorized;

					if(!line_split.fail()) {
						if(authorized == "*" || _all_authorized) {
							_authorizeds.clear();
							_all_authorized = true;
						} else {
							_authorizeds.push_back(authorized);
						}
					} else {
						return false;
					}
				} else if(tag == "OsmFile") {
					std::string osm_file;

					line_split >> osm_file;

					if(!line_split.fail()) {
						_osm_file = osm_file;
					} else {
						return false;
					}
				} else {
					return false;
				}
			}
		}
	}

	return true;
}

bool ConfLoader::allAuthorized() const
{
	return _all_authorized;
}

std::vector<short int> ConfLoader::getPorts() const
{
	return _ports;
}

std::vector<std::string> ConfLoader::getAuthorizeds() const
{
	return _authorizeds;
}

std::string ConfLoader::getOsmFile() const
{
	return _osm_file;
}
