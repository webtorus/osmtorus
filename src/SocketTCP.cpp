#include "include/SocketTCP.hpp"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

SocketTCP::SocketTCP()
{
	_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(_sock == -1) {
		std::cerr << "[ERROR]Class SocketTCP: the creation of the socket failed" << std::endl;
	}
}

bool SocketTCP::binding(short int port)
{
	int bind_return = -1;

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(port);

	if(_sock != -1) {
		bind_return = bind(_sock, (const struct sockaddr*)&_addr, sizeof(struct sockaddr_in));
	}

	if(bind_return == -1) {
		std::cerr << "[ERROR]Class SocketTCP: bind failed" << std::endl;

		return false;
	}

	return true;
}

bool SocketTCP::connecting(const char* hostname, short int port)
{
	int connect_return = -1;
	struct hostent *ht = NULL;

	ht = gethostbyname(hostname);

	if(ht == NULL) {
		std::cerr << "[ERROR]Class SocketTCP: get host by name failed" << std::endl;

		return false;
	}

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = ((struct in_addr*)(ht->h_addr))->s_addr;
	_addr.sin_port = htons(port);

	if(_sock != -1) {
		connect_return = connect(_sock, (const struct sockaddr*)&_addr, sizeof(struct sockaddr_in));
	}

	if(connect_return == -1) {
		std::cerr << "[ERROR]Class SocketTCP: connect failed" << std::endl;

		return false;
	}

	return true;
}

bool SocketTCP::listening(unsigned int nb_listener)
{
	int listen_return = -1;

	if(_sock != -1) {
		listen_return = listen(_sock, nb_listener);
	}

	if(listen_return == -1) {
		std::cerr << "[ERROR]Class SocketTCP: listen failed" << std::endl;

		return false;
	}

	return true;
}

bool SocketTCP::accepting(SocketTCP& socket)
{
	int accept_return = -1;
	socklen_t len = sizeof(struct sockaddr_in);
	
	if(_sock != -1) {
		accept_return = accept(_sock, (struct sockaddr*)&_addr, &len);
	}

	if(accept_return == -1) {
		std::cerr << "[ERROR]Class SocketTCP: accept failed" << std::endl;

		return false;
	}

	socket = *this;
	socket.setSock(accept_return);

	return true;
}

bool SocketTCP::shutdowning(int comment)
{
	int shutdown_return = -1;

	if(_sock != -1) {
		shutdown_return = shutdown(_sock, comment);
	}

	if(shutdown_return == -1) {
		std::cerr << "[ERROR]Class SocketTCP: shutdown failed" << std::endl;

		return false;
	}

	return true;
}

bool SocketTCP::closing()
{
	int close_return = -1;

	if(_sock != -1) {
		close_return = close(_sock);
	}

	if(close_return == -1) {
		std::cerr << "{ERROR]Class SocketTCP: close failed" << std::endl;

		return false;
	}

	return true;
}

int SocketTCP::sending(void* message, unsigned int size)
{
	int send_return = -1;

	if(_sock != -1) {
		send_return = send(_sock, message, size, 0);
	}

	if(send_return == -1) {
		std::cerr << "[ERROR]Class SocketTCP: send failed" << std::endl;
	}

	return send_return;
}

int SocketTCP::sending(char message)
{
	return this->sending((void*)&message, sizeof(char));
}

int SocketTCP::sending(int message)
{
	return this->sending((void*)&message, sizeof(int));
}

int SocketTCP::sending(float message)
{
	return this->sending((void*)&message, sizeof(float));
}

int SocketTCP::sending(double message)
{
	return this->sending((void*)&message, sizeof(double));
}

int SocketTCP::receive(void* message, unsigned int size)
{
	int recv_return = -1;

	if(_sock != -1) {
		recv_return = recv(_sock, message, size, 0);
	}

	if(recv_return == -1) {
		std::cerr << "[ERROR]Class SocketTCP: receive failed" << std::endl;
	}

	return recv_return;
}

int SocketTCP::receive(char* message)
{
	return this->receive((void*)message, sizeof(char));;
}

int SocketTCP::receive(int* message)
{
	return this->receive((void*)message, sizeof(int));
}

int SocketTCP::receive(float* message)
{
	return this->receive((void*)message, sizeof(float));
}

int SocketTCP::receive(double* message)
{
	return this->receive((void*)message, sizeof(double));
}

int SocketTCP::getSock() const
{
	return _sock;
}

struct sockaddr_in SocketTCP::getAddr() const
{
	return _addr;
}

std::string SocketTCP::getClientIp() const
{
	return std::string(inet_ntoa(_addr.sin_addr));
}

void SocketTCP::setSock(int sock)
{
	_sock = sock;
}
