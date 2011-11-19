
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
	virtual void Lock(int p);
	virtual void Unlock(int p);
	static void SetTable(Semaphore** table, int count);
	static Semaphore* GetSemaphore(int s);
	int locking_process;
	int locked_process;
protected:
	pthread_mutex_t mutex;
	static int count;
	static Semaphore** table;
};

#endif /* __SEMAPHORE_H */
