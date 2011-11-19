
#include "Semaphore.h"

int Semaphore::count = 0;
Semaphore** Semaphore::table = NULL;

Semaphore::Semaphore()
{
	pthread_mutex_init(&mutex, NULL);
}

Semaphore::~Semaphore()
{
	pthread_mutex_destroy(&mutex);
}

void Semaphore::Lock(int p)
{
	pthread_mutex_lock(&mutex);
}

void Semaphore::Unlock(int p)
{
	pthread_mutex_unlock(&mutex);
}

void Semaphore::SetTable(Semaphore** table, int count)
{
	Semaphore::table = table;
	Semaphore::count = count;
}

Semaphore* Semaphore::GetSemaphore(int s)
{
	return Semaphore::table[s];
}
