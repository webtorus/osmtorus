#include "include/HttpRequesterReaderServer.hpp"
#include "include/SocketTCP.hpp"

HttpRequesterReaderServer::HttpRequesterReaderServer()
{

}

bool HttpRequesterReaderServer::run(SocketTCP& requester_socket)
{
//requester_socket.receive(); todo
/*
	std::string request;
	std::string response;
	std::stringstream request_ss;

	request_ss << request;
	
	while(!request_ss.eof() && request_ss.peek() != '?') {
		request_ss >> buffer;
		cmd += buffer;
	}
	if(!request_ss.eof()) {
		request_ss >> buffer;
	}

	if(cmd == "route") {
		bool hasLat1 = false;
		bool hasLng1 = false;
		bool hasLat2 = false;
		bool hasLng2 = false;
		bool hasType = false;
		double lat1 = 0.0;
		double lng1 = 0.0;
		double lat2 = 0.0;
		double lng2 = 0.0;
		short type = 0;

		while(!request_ss.eof()) {
			std::string arg;

			while(!request_ss.eof() && request_ss.peek() != '=') {
				request_ss >> buffer;
				arg += buffer;
			}
			if(request_ss.eof()) {
				break;
			}

			if(arg == "lat1") {
				if(!hasLat1) {
					request_ss >> lat1;
					if(request_ss.fail()) {
						break;
					}
					hasLat1 = true;
				} else {
					hasLat1 = false;
					break;
				}
			} else if(arg == "lng1") {
				if(!hasLng1) {
					request_ss >> lng1;
					if(request_ss.fail()) {
						break;
					}
					hasLng1 = true;
				} else {
					hasLng1 = false;
					break;
				}
			} else if(arg == "lat2") {
				if(!hasLat2) {
					request_ss >> lat2;
					if(request_ss.fail()) {
						break;
					}
					hasLat2 = true;
				} else {
					hasLat2 = false;
					break;
				}
			} else if(arg == "lng2") {
				if(!hasLng2) {
					request_ss >> lng2;
					if(request_ss.fail()) {
						break;
					}
					hasLng2 = true;
				} else {
					hasLng2 = false;
					break;
				}
			} else if(arg == "type") {
				if(!hasType) {
					request_ss >> type;
					if(request_ss.fail()) {
						break;
					}
					hasType = true;
				} else {
					hasType = false;
					break;
				}
			} else {
				break;
			}
		}

		//response = route(lat1, lng1, lat2, lng2, type);
		requester_socket.sending(response);
	}
*/
	return false;
}
