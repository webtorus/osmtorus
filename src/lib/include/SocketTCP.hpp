#ifndef SOCKETTCP_HPP
#define SOCKETTCP_HPP

#include <netdb.h>
#include <string>

class SocketTCP
{
	public:
		SocketTCP();

		bool binding(short int port = 0);
		bool connecting(const char* hostname, short int port);
		bool listening(unsigned int nb_listener = 1);
		bool accepting(SocketTCP& socket);
		bool shutdowning(int comment);
		bool closing();

		int sending(void* message, unsigned int size = 1);
		int sending(std::string& message);
		int sending(int message);
		int sending(float message);
		int sending(double message);

		int receive(void* message, unsigned int size = 1);
		int receive(std::string& message);
		int receive(int* message);
		int receive(float* message);
		int receive(double* message);
		
		int getSock() const;
		struct sockaddr_in getAddr() const;

		void setSock(int sock);

	private:
		int _sock;
		struct sockaddr_in _addr;
};

#endif
