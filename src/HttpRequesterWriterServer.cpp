#include <string>
#include "include/HttpRequesterWriterServer.hpp"
#include "include/SocketTCP.hpp"

using namespace std;

HttpRequesterWriterServer::HttpRequesterWriterServer()
{

}

void HttpRequesterWriterServer::run(SocketTCP& requester_socket, const string& result)
{
	requester_socket.sending(result);
}
