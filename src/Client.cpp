#include "include/Client.hpp"
#include "include/SocketTCP.hpp"
#include <iostream>
#include <sstream>
#include <string>

void Client::run(int argc, char* argv[])
{
	if(argc >= 3) {
		std::ostringstream request_oss;
		std::string request;
		std::string response;
		SocketTCP client_socket;

		for(int i = 2; i <= argc; i++) {
			request_oss << argv[i];
			request_oss << " ";
		}

		request = request_oss.str();

		if(request.size() > 0) {
			request.erase(request.size() - 1);
		}

		client_socket.binding();
		client_socket.connecting(argv[0], (short int)atoi(argv[1]));
		//client_socket.sending(); todo
		//client_socket.receive(); todo
		client_socket.closing();
	}
}

