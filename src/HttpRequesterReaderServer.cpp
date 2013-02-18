#include <iostream>
#include <map>
#include "include/HttpRequesterReaderServer.hpp"
#include "include/SocketTCP.hpp"
#include "include/Utils.hpp"
#include "include/WayType.hpp"

using namespace std;

HttpRequesterReaderServer::HttpRequesterReaderServer()
{

}

double HttpRequesterReaderServer::getLatSource() const
{
	return _lat_source;
}

double HttpRequesterReaderServer::getLonSource() const
{
	return _lon_source;
}

double HttpRequesterReaderServer::getLatTarget() const
{
	return _lat_target;
}

double HttpRequesterReaderServer::getLonTarget() const
{
	return _lon_target;
}

short HttpRequesterReaderServer::getRoutingType() const
{
	return _routing_type;
}

bool HttpRequesterReaderServer::run(SocketTCP& requester_socket)
{
	char c;
	string request;
	
	requester_socket.receive(&c);
	while (c != '\0') {
		request += c;
		requester_socket.receive(&c);
	}
	
	cout << request << endl;	
	// request = route?source=_lat_,_lon_&target=_lat_,_lon&type=_t1_|_t2_

	string request_type = request.substr(0, request.find_first_of('?') + 1);

	if (request_type != "route?") {
		cout << "Request type = " << request_type << endl;
		return false;
	}

	map<string, string> request_params;

	string params_string = request.substr(request_type.size())  + '&';
	unsigned pos_and = params_string.find_first_of('&');
	while (pos_and < params_string.size()) {
		string param_string = params_string.substr(0, pos_and);

		unsigned pos_equal = param_string.find_first_of('=');
		string key = param_string.substr(0, pos_equal);
		string value = params_string.substr(pos_equal + 1);
		request_params[key] = value;

		params_string = params_string.substr(pos_and + 1);
	}

	vector<string> tokens;
	// Source
	tokens = split(request_params["source"], ',');
	_lat_source = atof(tokens[0].c_str());
	_lon_source = atof(tokens[1].c_str());

	// Target
	tokens = split(request_params["target"], ',');
	_lat_target = atof(tokens[0].c_str());
	_lon_target = atof(tokens[1].c_str());

	// Type
	short type = 0;
	tokens = split(request_params["type"], '|');
	for (unsigned i = 0; i < tokens.size(); i++) {
		if (tokens[i] == "car") {
			type |= CAR_WAY;
		} else if (tokens[i] == "foot") {
			type |= FOOT_WAY;
		} else if (tokens[i] == "tram") {
			type |= TRAM_WAY;
		} else if (tokens[i] == "bus") {
			type |= BUS_WAY;
		} else if (tokens[i] == "cycle") {
			type |= CYCLE_WAY;
		}
	}

	_routing_type = type;

	return false;
}
