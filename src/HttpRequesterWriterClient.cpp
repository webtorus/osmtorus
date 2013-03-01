#include "include/HttpRequesterWriterClient.hpp"
#include <sstream>
#include <string>

HttpRequesterWriterClient::HttpRequesterWriterClient()
{

}

bool HttpRequesterWriterClient::parse(std::string url)
{
	unsigned int i = 0;
	std::string protocol;
	std::string port_string;

	_server_port = -1;
	_server_name = "";
	_request = "";
	
	for(; i < url.size(); i++) {
		if(url[i] == ':') {
			break;
		}
		protocol += url[i];
	}

	if(protocol != "http") {
		return false;
	}

	for(unsigned int j = 0; j < 2; j++) {
		if(++i < url.size() && url[i] != '/') {
			return false;
		}
	}

	for(++i; i < url.size(); i++) {
		if(url[i] == '/') {
			_server_port = 80;
			break;
		} else if(url[i] == ':') {
			for(++i; i < url.size(); i++) {
				if(url[i] == '/') {
					short int port;
					std::stringstream port_ss;

					port_ss << port_string;
					port_ss >> port;

					if(!port_ss.fail()) {
						_server_port = port;
					}

					break;
				}
				port_string += url[i];
			}
			break;
		}
		_server_name += url[i];
	}

	if(_server_port == -1 || _server_name == "") {
		return false;
	}

	_request += '/';

	for(++i; i < url.size(); i++) {
		_request += url[i];
	}

	return true;
}

bool HttpRequesterWriterClient::run(SocketTCP& client_socket, std::string cmd)
{
	std::string request;

	request += "GET ";
	request += cmd;
	request += "HTTP/1.1\r\n\r\n";

	for(unsigned int i = 0; i < request.size(); i++) {
		client_socket.sending(request[i]);
	}

	return true;
}

short int  HttpRequesterWriterClient::getServerPort() const
{
	return _server_port;
}

std::string  HttpRequesterWriterClient::getServerName() const
{
	return _server_name;
}

std::string  HttpRequesterWriterClient::getRequest() const
{
	return _request;
}
