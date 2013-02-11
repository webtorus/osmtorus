#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	public:
		void run(int argc, char* argv[]);
		void loadosm(const char* hostname, short int port, std::string request);
		void route(const char* hostname, short int port, std::string request);
};

#endif
