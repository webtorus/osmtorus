#ifndef HTTP_REQUESTER_WRITER_CLIENT_HPP
#define HTTP_REQUESTER_WRITER_CLIENT_HPP

#include "SocketTCP.hpp"
#include <string>

class HttpRequesterWriterClient
{
	public:
		HttpRequesterWriterClient();
		bool parse(std::string url);
		bool run(SocketTCP& client_socket, std::string cmd);

		short int getServerPort() const;
		std::string getServerName() const;
		std::string getRequest() const;

	private:
		short int _server_port;
		std::string _server_name;
		std::string _request;
};

#endif
