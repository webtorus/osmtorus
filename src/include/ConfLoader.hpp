#ifndef CONF_LOADER_HPP
#define CONF_LOADER_HPP

#include <string>
#include <vector>

class ConfLoader
{
	public:
		ConfLoader();
		bool run(bool is_daemon);

		bool allAuthorized() const;
		std::vector<short int> getPorts() const;
		std::vector<std::string> getAuthorizeds() const;
		std::string getOsmFile() const;

	private:
		bool _all_authorized;
		std::string _osm_file;
		std::vector<short int> _ports;
		std::vector<std::string> _authorizeds;
};

#endif
