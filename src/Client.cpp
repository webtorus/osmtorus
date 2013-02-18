#include "include/Client.hpp"
#include "include/HttpRequesterWriterClient.hpp"
#include "include/SocketTCP.hpp"
#include <iostream>
#include <sstream>
#include <string>

void Client::run(int argc, char* argv[])
{
	if(argc == 1) {
		HttpRequesterWriterClient http_requester_writer_client;
		SocketTCP client_socket;

		http_requester_writer_client.run(std::string(argv[0]));

		client_socket.binding();
		client_socket.connecting(argv[0], (short int)atoi(argv[1]));

		client_socket.closing();
	} else {
		std::cerr << "Invalid Url" << std::endl;
		std::cerr << "For Example: ./osmtorus_client \"http://localhost:7777/route?lat1=32.1&lng1=3.23&lat2=33.12&lng2=3.234&type=footway|cycleway\"" << std::endl;
	}
}

