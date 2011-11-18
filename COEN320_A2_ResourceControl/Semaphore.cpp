
#include "Semaphore.h"

Semaphore::Semaphore()
{
	pthread_mutex_init(&mutex, NULL);
}

Semaphore::~Semaphore()
{
	pthread_mutex_destroy(&mutex);
}

void Semaphore::Lock(int p, Semaphore* s[])
{
	pthread_mutex_lock(&mutex);
}

void Semaphore::Unlock(int p)
{
	pthread_mutex_unlock(&mutex);
}
