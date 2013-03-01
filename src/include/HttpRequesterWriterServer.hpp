#ifndef HTTP_REQUESTER_WRITER_SERVER_HPP
#define HTTP_REQUESTER_WRITER_SERVER_HPP

#include <string>
#include "SocketTCP.hpp"

using namespace std;

class HttpRequesterWriterServer
{
	public:
		HttpRequesterWriterServer();
		void run(SocketTCP& requester_socket, const string& result);
};

#endif
