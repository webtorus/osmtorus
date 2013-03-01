#ifndef HTTP_REQUESTER_READER_CLIENT_HPP
#define HTTP_REQUESTER_READER_CLIENT_HPP

#include "SocketTCP.hpp"
#include <string>

class HttpRequesterReaderClient
{
	public:
		HttpRequesterReaderClient();
		bool run(SocketTCP& client_socket);

		std::string getResponse() const;

	private:
		std::string _response;
};

#endif
