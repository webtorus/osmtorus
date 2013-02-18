#include "include/Client.hpp"
#include "include/HttpRequesterReaderClient.hpp"
#include "include/HttpRequesterWriterClient.hpp"
#include "include/SocketTCP.hpp"
#include <iostream>
#include <sstream>
#include <string>

void Client::run(int argc, char* argv[])
{
	bool http_requester_writer_client_return = false;

	if(argc == 1) {
		HttpRequesterReaderClient http_requester_reader_client;
		HttpRequesterWriterClient http_requester_writer_client;
		SocketTCP client_socket;

		http_requester_writer_client_return = http_requester_writer_client.run(std::string(argv[0]));

		if(http_requester_writer_client_return) {
			std::string request;

			client_socket.binding();
			client_socket.connecting(http_requester_writer_client.getServerName().c_str(), http_requester_writer_client.getServerPort());

			request = http_requester_writer_client.getRequest();
			client_socket.sending(request);
			http_requester_reader_client.run(client_socket);
std::cout << http_requester_reader_client.getResponse() << std::endl;
			client_socket.closing();
		}
	}

	if(argc != 1 || !http_requester_writer_client_return) {
		std::cerr << "Invalid Url" << std::endl;
		std::cerr << "For Example: ./osmtorus_client \"http://localhost:7777/route?source=32.1,3.23&target=33.12,3.234&type=foot|cycle\"" << std::endl;
	}
}

