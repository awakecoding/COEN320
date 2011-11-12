
#ifndef DATA_H_
#define DATA_H_

#include <pthread.h>

class Data
{
public:
	Data();
	virtual ~Data();

	double x, y, z;
	double vx, vy, vz;

	void Lock();
	void Unlock();

private:
	pthread_mutex_t mutex;
};

#endif /* DATA_H_ */
