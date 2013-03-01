#include "include/Cond.hpp"
#include "include/Mutex.hpp"
#include <pthread.h>
#include <cstdlib>
#include <iostream>

Cond::Cond()
{
	int pthread_cond_init_return = 0;

	pthread_cond_init_return = pthread_cond_init(&_self, NULL);

	if(pthread_cond_init_return != 0) {
		std::cerr << "[ERROR]Class Cond: the creation of the cond failed" << std::endl;
	}
}

Cond::~Cond()
{
	int pthread_cond_destroy_return = 0;

	pthread_cond_destroy_return = pthread_cond_destroy(&_self);

	if(pthread_cond_destroy_return != 0) {
		std::cerr << "[ERROR]Class Cond: the destruction of the cond failed" << std::endl;
	}
}

int Cond::wait(Mutex& mutex)
{
	return pthread_cond_wait(&_self, &(mutex.getSelf()));
}

int Cond::broadcast()
{
	return pthread_cond_broadcast(&_self);
}

int Cond::signal()
{
	return pthread_cond_signal(&_self);
}

pthread_cond_t Cond::getSelf() const
{
	return _self;
}
