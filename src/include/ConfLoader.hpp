#ifndef CONF_LOADER_HPP
#define CONF_LOADER_HPP

#include <string>
#include <set>

class ConfLoader
{
	public:
		ConfLoader();
		bool run(bool is_daemon);

		bool allAuthorized() const;
		std::set<short int> getPorts() const;
		std::set<std::string> getAuthorizeds() const;
		std::string getOsmFile() const;

	private:
		bool _all_authorized;
		std::string _osm_file;
		std::set<short int> _ports;
		std::set<std::string> _authorizeds;
};

#endif
