#include "include/Server.hpp"
#include "include/ConfLoader.hpp"
#include "include/HttpRequesterReaderServer.hpp"
#include "include/HttpRequesterWriterServer.hpp"
#include "include/JsonResponse.hpp"
#include "include/OsmLoader.hpp"
#include "include/RoutingTracer.hpp"
#include "include/SocketTCP.hpp"
#include "include/Thread.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>

void Server::run(int argc, char* argv[])
{
	if(argc == 0 || (argc == 1 && std::string(argv[0]) == "-d")) {
		bool conf_loader_return = false;

		if(argc == 0) {
			conf_loader_return = _conf_loader.run(false);
		} else {
			conf_loader_return = _conf_loader.run(true);
		}

		if(conf_loader_return) {
			std::set<short int> ports = _conf_loader.getPorts();

			if(_conf_loader.getOsmFile() != "") {
				bool osm_loader_return = false;

				osm_loader_return = _osm_loader.run(_conf_loader.getOsmFile());

				if(osm_loader_return) {
					for(short int port: ports) {
						Thread listener_thread;

						listener_mutex.lock();
						listener_thread.create(Server::listener, (void*)&port);
						listener_cond.wait(listener_mutex);
						listener_mutex.unlock();
						threads.push_back(listener_thread);
					}
				}
			}
		} else {
			std::cerr << "Invalid Conf File" << std::endl;
		}
	}
	
	for(unsigned int i = 0; i < threads.size(); i++) {
		threads[i].join();
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
		bool client_ip_authorized = false;
		std::string client_ip;

		requester_mutex.lock();
		listener_socket.accepting(requester_socket);

		client_ip = requester_socket.getClientIp();

		for(std::string next_ip: _conf_loader.getAuthorizeds()) {
			if(next_ip == client_ip) {
				client_ip_authorized = true;
				break;
			}
		}

		if(client_ip_authorized || _conf_loader.allAuthorized()) {
			requester_thread.create(Server::requester, (void*)&requester_socket);
		}
		
		client_ip_authorized = false;

		requester_cond.wait(requester_mutex);
		requester_mutex.unlock();
	}

	listener_socket.closing();
	Thread::exit();

	return EXIT_SUCCESS;
}

void* Server::requester(void* arg)
{
	bool http_requester_reader_server_return = false;
	bool json_response_return = false;
	bool routing_tracer_return = false;
	std::string response;
	HttpRequesterReaderServer http_requester_reader_server;
	HttpRequesterWriterServer http_requester_writer_server;
	JsonResponse json_response;
	RoutingTracer routing_tracer;
	SocketTCP requester_socket;

	requester_mutex.lock();
	requester_socket = *((SocketTCP*)arg);
	requester_cond.signal();
	requester_mutex.unlock();

	http_requester_reader_server_return = http_requester_reader_server.run(requester_socket);

	if(http_requester_reader_server_return) {
		routing_tracer_return = routing_tracer.run(
			http_requester_reader_server.getLatSource(),
			http_requester_reader_server.getLonSource(),
			http_requester_reader_server.getLatTarget(),
			http_requester_reader_server.getLonTarget(),
			http_requester_reader_server.getRoutingType(),
			_osm_loader.getRoutingGraph()
		);
	}

	if(routing_tracer_return) {
		json_response_return = json_response.run(
			http_requester_reader_server.getLatSource(),
			http_requester_reader_server.getLonSource(),
			http_requester_reader_server.getLatTarget(),
			http_requester_reader_server.getLonTarget(),
			routing_tracer.getRoutingEdges(),
			_osm_loader.getRoutingGraph()
		);
	}

	if(json_response_return) {
		response = json_response.getResponse();
	}

	http_requester_writer_server.run(requester_socket, response);

	requester_socket.closing();

	Thread::exit();

	return EXIT_SUCCESS;
}

Cond Server::listener_cond;

Cond Server::requester_cond;

Mutex Server::listener_mutex;

Mutex Server::requester_mutex;

ConfLoader Server::_conf_loader;

OsmLoader Server::_osm_loader;

std::vector<Thread> Server::threads;
