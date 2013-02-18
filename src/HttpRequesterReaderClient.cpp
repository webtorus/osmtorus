#include "include/HttpRequesterReaderClient.hpp"
#include <iostream>

HttpRequesterReaderClient::HttpRequesterReaderClient()
{

}

bool HttpRequesterReaderClient::run(SocketTCP& client_socket)
{
	char buffer;

	_response = "";

	client_socket.receive(&buffer);
	while(buffer != '\0') {
		_response += buffer;
		client_socket.receive(&buffer);
	}

	if(_response.size() == 0) {
		return false;
	}

	return true;
}

std::string HttpRequesterReaderClient::getResponse() const
{
	return _response;
}
