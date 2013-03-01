#ifndef THREAD_HPP
#define THREAD_HPP

#include <pthread.h>

typedef void* (*TypeFunction)(void*);
typedef void* TypeData;

class Thread
{
	public:
		Thread(TypeFunction function = NULL, TypeData data = NULL);

		void create(TypeFunction function = NULL, TypeData data = NULL);
		bool equal(Thread const& other_thread) const;
		int cancel();
		int join(TypeData* return_data = NULL);

		pthread_t getSelf() const;

		static pthread_t self();
		static void exit(TypeData return_data = NULL);

	private:
		TypeFunction _function;
		TypeData _data;
		pthread_t _self;
};

bool operator==(Thread const& thread1, Thread const& thread2);

#endif
