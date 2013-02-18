#ifndef HTTP_REQUESTER_READER_SERVER_HPP
#define HTTP_REQUESTER_READER_SERVER_HPP

#include "SocketTCP.hpp"

class HttpRequesterReaderServer
{
	public:
		HttpRequesterReaderServer();

		bool run(SocketTCP& requester_socket);
};

#endif
