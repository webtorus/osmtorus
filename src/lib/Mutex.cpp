#include "include/Mutex.hpp"
#include <pthread.h>
#include <cstdlib>
#include <iostream>

Mutex::Mutex()
{
	int pthread_mutex_init_return = 0;

	pthread_mutex_init_return = pthread_mutex_init(&_self, NULL);

	if(pthread_mutex_init_return != 0) {
		std::cerr << "[ERROR]Class Mutex: the creation of the mutex failed" << std::endl;
	}
}

Mutex::~Mutex()
{
	int pthread_mutex_destroy_return = 0;

	pthread_mutex_destroy_return = pthread_mutex_destroy(&_self);

	if(pthread_mutex_destroy_return != 0) {
		std::cerr << "[ERROR]Class Mutex: the destruction of the mutex failed" << std::endl;
	}
}

void Mutex::lock()
{
	pthread_mutex_lock(&_self);
}

int Mutex::trylock()
{
	return pthread_mutex_trylock(&_self);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(&_self);
}

pthread_mutex_t& Mutex::getSelf()
{
	return _self;
}
