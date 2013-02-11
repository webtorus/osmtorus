#ifndef SERVER_HPP
#define SERVER_HPP

#include "../lib/include/Cond.hpp"
#include "../lib/include/Mutex.hpp"
#include <string>

class Server
{
	public:
		void run();
		static void* listener(void* arg);
		static void* requester(void* arg);
		static std::string loadosm(std::string& osm);
		static std::string route(std::string& arg);

	private:
		static Cond listener_cond;
		static Cond requester_cond;
		static Mutex listener_mutex;
		static Mutex requester_mutex;
};

#endif
