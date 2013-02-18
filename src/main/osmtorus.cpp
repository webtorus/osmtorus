#include "../include/Server.hpp"

int main(int argc, char* argv[])
{
	Server server;

	server.run(--argc, ++argv);

	return 0;
}
