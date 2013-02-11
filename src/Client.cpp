#include "include/Client.hpp"
#include "lib/include/SocketTCP.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void Client::run(int argc, char* argv[])
{
	if(argc > 1) {
		std::string cmd(argv[0]);
		std::ostringstream request;

		for(int i = 0; i <= argc; i++) {
			request << argv[i];
			request << " ";
		}

		if(request.str().size() > 0) {
			request.str(request.str().erase(request.str().size() - 1));
		}

		if(cmd == "loadosm") {
			loadosm((const char*)"localhost", (short int)7777, request.str());
		} else if(cmd == "route") {
			route((const char*)"localhost", (short int)7777, request.str());
		} else {
			std::cout << "invalid command" << std::endl;
		}		
	}
}

void Client::loadosm(const char* hostname, short int port, std::string request)
{
	std::string cmd;
	std::string filename;
	std::string response;
	std::string osm;
	std::stringstream flow;
	SocketTCP client_socket;

	flow << request;
	flow >> cmd;
	flow >> filename;	

	std::ifstream file(filename);
 
	if(file) {
		std::stringstream buffer;

		buffer << cmd << " " << file.rdbuf();
		file.close();

		request = buffer.str();
//std::cout << request << std::endl;
		client_socket.binding();
		client_socket.connecting(hostname, port);
		client_socket.sending(request);
		client_socket.receive(response);
		client_socket.closing();

//std::cout << response << std::endl;
	}
}

void Client::route(const char* hostname, short int port, std::string request)
{
	SocketTCP client_socket;
	std::string response;

	client_socket.binding();
	client_socket.connecting(hostname, port);
	client_socket.sending(request);
	client_socket.receive(response);
	client_socket.closing();

std::cout << response << std::endl;
}
