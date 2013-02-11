#include "../include/Client.hpp"

int main(int argc, char* argv[])
{
	Client client;

	client.run(--argc, ++argv);

	return 0;
}
