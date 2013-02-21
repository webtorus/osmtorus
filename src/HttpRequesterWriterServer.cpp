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
	string response;

	response += "HTTP/1.1 200 OK\r\n";
	response += "Server: osmtorus\r\n";
	response += "Content-Length: ";
	response += result.size();
	response += "\r\nContent-Type: text/html\r\n\r\n";
	response += result;
	response += "\r\n";

	for(unsigned int i = 0; i < response.size(); i++) {
		requester_socket.sending(response[i]);
	}
}
