#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <pthread.h>

class Mutex
{
	public:
		Mutex();
		~Mutex();

		void lock();
		int trylock();
		void unlock();

		pthread_mutex_t& getSelf();

	private:
		pthread_mutex_t _self;
};

#endif
