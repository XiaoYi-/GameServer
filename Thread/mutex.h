#ifndef MUTEX_H
#define MUTEX_H

#include <iostream>
#include <pthread.h>

class Mutex{
public:
	Mutex(){
		pthread_mutex_init(&_mutex,NULL);
	}
	~Mutex(){
		try{pthread_mutex_destroy(&_mutex);}
		catch(...){
			//std::abort();
		}
	}
	void lock(){ pthread_mutex_lock(&_mutex); }
	void unlock(){ pthread_mutex_unlock(&_mutex); }

private:
	Mutex(const Mutex& rhs){} //
	Mutex& operator = (const Mutex& rhs){}//
	pthread_mutex_t _mutex;
};

class MutexLock{
public:
	MutexLock(Mutex& m):_mutex(m){ _mutex.lock(); }
	~MutexLock(){ _mutex.unlock(); }
private:
	Mutex& _mutex;
};

#endif