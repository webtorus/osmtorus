#include "lib/include/SocketTCP.hpp"
#include "lib/include/Thread.hpp"
#include "include/Server.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void Server::run()
{
	Thread listener_thread;
	std::ifstream conf_file("/etc/osmtorus/osmtorus.conf");

	if(conf_file) {
		std::string line;
		
		while(std::getline(conf_file, line)) {
			std::istringstream line_split(line);
			std::string tag;

			if(line_split) {
				line_split >> tag;

				if(tag == "Listen") {
					short int port = -1;

					while(line_split) {
						line_split >> port;

						if(port != -1) {
							listener_mutex.lock();
							listener_thread.create(Server::listener, (void*)&port);
							listener_cond.wait(listener_mutex);
							listener_mutex.unlock();
							port = -1;
						}
					}
				} else if(tag == "Authorized") {

				}
			}
		}

		Thread::exit();
	}
}

void* Server::listener(void* arg)
{
	short int port = 0;
	SocketTCP listener_socket;
	SocketTCP requester_socket;
	Thread requester_thread;

	listener_mutex.lock();
	port = *((short int*)arg);
	listener_cond.signal();
	listener_mutex.unlock();
	listener_socket.binding(port);
	listener_socket.listening(10);

	while(true) {
		requester_mutex.lock();
		listener_socket.accepting(requester_socket);
		requester_thread.create(Server::requester, (void*)&requester_socket);
		requester_cond.wait(requester_mutex);
		requester_mutex.unlock();
	}

	listener_socket.closing();
	Thread::exit();

	return EXIT_SUCCESS;
}

void* Server::requester(void* arg)
{
	std::string request;
	std::string response;
	std::string cmd;
	std::stringstream flow;
	SocketTCP requester_socket;

	requester_mutex.lock();
	requester_socket = *((SocketTCP*)arg);
	requester_cond.signal();
	requester_mutex.unlock();
	requester_socket.receive(request);
//std::cout << request << std::endl;
	flow << request;
	flow >> cmd;

	if(cmd == "loadosm") {
		std::string osm;

		osm.assign(flow.str(), flow.str().size() - (flow.rdbuf()->in_avail() - 1), flow.rdbuf()->in_avail() - 1);
//std::cout << osm << std::endl;
		response = loadosm(osm);
		requester_socket.sending(response);
	} else if(cmd == "route") {
		std::string arg;

		arg.assign(flow.str(), flow.str().size() - (flow.rdbuf()->in_avail() - 1), flow.rdbuf()->in_avail() - 1);
		response = route(arg);
		requester_socket.sending(response);
	}

	requester_socket.closing();
	Thread::exit();

	return EXIT_SUCCESS;
}

std::string Server::loadosm(std::string& osm)
{
	std::string return_info;

	//to do
	return_info = osm;

	return return_info;
}

std::string Server::route(std::string& arg)
{
	std::string geojson;

	//to do
	geojson = arg;

	return geojson;
}

Cond Server::listener_cond;

Cond Server::requester_cond;

Mutex Server::listener_mutex;

Mutex Server::requester_mutex;
