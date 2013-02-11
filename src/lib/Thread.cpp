#include "include/Thread.hpp"
#include <pthread.h>
#include <cstdlib>
#include <iostream>

Thread::Thread(TypeFunction function, TypeData data)
{
	_function = function;
	_data = data;
	_self = 0;
}

void Thread::create(TypeFunction function, TypeData data)
{
	if(function != NULL) {
		_function = function;
		_data = data;
	}

	if(_function != NULL) {
		int pthread_create_return = 0;

		pthread_create_return = pthread_create(&_self, NULL, _function, _data);

		if(pthread_create_return != 0) {
			std::cerr << "[ERROR]Class Thread: the creation of the thread failed" << std::endl;
		}
	}
}

bool Thread::equal(Thread const& other_thread) const
{
	return (bool)pthread_equal(_self, other_thread.getSelf());
}

int Thread::cancel()
{
	return pthread_cancel(_self);
}

int Thread::join(TypeData* return_data)
{
	return pthread_join(_self, return_data);
}

pthread_t Thread::getSelf() const
{
	return _self;
}

pthread_t Thread::self()
{
	return pthread_self();
}

void Thread::exit(TypeData return_data)
{
	pthread_exit(return_data);
}

bool operator==(Thread const& thread1, Thread const& thread2)
{
	return thread1.equal(thread2);
}
