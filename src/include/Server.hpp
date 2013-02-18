#ifndef SERVER_HPP
#define SERVER_HPP

#include "Cond.hpp"
#include "ConfLoader.hpp"
#include "OsmLoader.hpp"
#include "Mutex.hpp"
#include <string>

class Server
{
	public:
		void run(int argc, char* argv[]);

	private:
		static void* listener(void* arg);
		static void* requester(void* arg);

		static Cond listener_cond;
		static Cond requester_cond;
		static Mutex listener_mutex;
		static Mutex requester_mutex;
		static ConfLoader _conf_loader;
		static OsmLoader _osm_loader;
};

#endif
