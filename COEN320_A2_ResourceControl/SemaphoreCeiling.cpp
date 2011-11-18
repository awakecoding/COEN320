
#include "SemaphoreCeiling.h"

SemaphoreCeiling::SemaphoreCeiling()
{
	pthread_mutex_init(&mutex, NULL);
}

SemaphoreCeiling::~SemaphoreCeiling()
{
	pthread_mutex_destroy(&mutex);
}

void SemaphoreCeiling::Lock(int p, Semaphore* s[])
{
	pthread_mutex_lock(&mutex);
}

void SemaphoreCeiling::Unlock(int p)
{
	pthread_mutex_unlock(&mutex);
}
