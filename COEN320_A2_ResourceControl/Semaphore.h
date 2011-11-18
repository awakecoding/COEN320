
#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <pthread.h>
#include <iostream>
using namespace std;

#include "Process.h"

class Semaphore
{
public:
	float pc;
	Semaphore();
	virtual ~Semaphore();
	virtual void Lock(int p, Semaphore* s[]);
	virtual void Unlock(int p);
protected:
	pthread_mutex_t mutex;
	int locking_process;
	int locked_process;
};

#endif /* __SEMAPHORE_H */
