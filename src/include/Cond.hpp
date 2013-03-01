#ifndef COND_HPP
#define COND_HPP

#include "Mutex.hpp"
#include <pthread.h>

class Cond
{
	public:
		Cond();
		~Cond();

		int wait(Mutex& mutex);
		int broadcast();
		int signal();

		pthread_cond_t getSelf() const;

	private:
		pthread_cond_t _self;
};

#endif
