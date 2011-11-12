
#include "Data.h"

Data::Data()
{
	x = y = z = 0.0;
	vx = vy = vz = 0.0;

	pthread_mutex_init(&mutex, NULL);
}

Data::~Data()
{
	pthread_mutex_destroy(&mutex);
}

void Data::Lock()
{
	pthread_mutex_lock(&mutex);
}

void Data::Unlock()
{
	pthread_mutex_unlock(&mutex);
}
