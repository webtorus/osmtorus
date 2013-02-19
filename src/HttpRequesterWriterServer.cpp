#include "include/HttpRequesterWriterServer.hpp"
#include "include/SocketTCP.hpp"
#include <iostream> 
#include <string>

using namespace std;

HttpRequesterWriterServer::HttpRequesterWriterServer()
{

}

void HttpRequesterWriterServer::run(SocketTCP& requester_socket, const string& result)
{
	requester_socket.sending(result);
}
