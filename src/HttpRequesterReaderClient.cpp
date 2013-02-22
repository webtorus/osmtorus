#include "include/HttpRequesterReaderClient.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

HttpRequesterReaderClient::HttpRequesterReaderClient()
{

}

bool HttpRequesterReaderClient::run(SocketTCP& client_socket)
{
	bool is_end_request = false;
	char c;
	std::string request;
	std::string attribute_header_request;
	std::vector<std::string> header_request;

	_response = "";
	
	while(!is_end_request) {
		client_socket.receive(&c);
		if(c == '\r') {
			header_request.push_back(attribute_header_request);
			client_socket.receive(&c);
			if(c == '\r') {
				is_end_request = true;
			} else if(c == '\n') {
				client_socket.receive(&c);
				if(c == '\r') {
					client_socket.receive(&c);
					if(c == '\n') {
						is_end_request = true;
					} else {
						return false;
					}
				} else {
					attribute_header_request = c;
				}
			} else {
				attribute_header_request = c;
			}
		} else if(c == '\n') {
			header_request.push_back(attribute_header_request);			
			if(c == '\n') {
				is_end_request = true;
			} else {
				attribute_header_request = c;
			}
		} else {
			attribute_header_request += c;
		}
	}

	if(header_request.size() > 0) {
		std::string protocol;
		std::string code;
		std::stringstream ss;

		ss << header_request[0];
		ss >> protocol;
		ss >> code;

		if(protocol == "HTTP/1.1") {
			if(code == "200") {
				unsigned int response_size = 0;
				std::string attribute_header;
std::cout << "200 OK" << std::endl;
				for(unsigned i = 1; i < header_request.size(); i++) {
					unsigned int j = 0;

					for(j = 0; j < header_request[i].size(); j++) {
						if(header_request[i][j] == ':') {
							break;
						}
						attribute_header += header_request[i][j];
					}

					if(attribute_header == "Content-Type") {
						std::stringstream ss2;

						for(; j < header_request[i].size(); j++) {
							ss2 << header_request[i][j];
						}

						ss2 >> response_size;

						if(ss2.fail()) {
							return false;
						}

						break;
					}
				}

				for(unsigned int i = 0; i < response_size; i++) {
					client_socket.receive(&c);
					_response += c;
				}

				return true;
			} else if(code == "404") {
std::cout << "404 Not Found" << std::endl;
				return true;
			}
		}
	}

	return false;
}

std::string HttpRequesterReaderClient::getResponse() const
{
	return _response;
}
