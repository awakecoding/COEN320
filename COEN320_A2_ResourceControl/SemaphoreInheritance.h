
#ifndef __SEMAPHORE_INHERITANCE_H
#define __SEMAPHORE_INHERITANCE_H

#include "Semaphore.h"

class SemaphoreInheritance : public Semaphore
{
public:
	SemaphoreInheritance();
	virtual ~SemaphoreInheritance();
	void Lock(int p, Semaphore* s[]);
	void Unlock(int p);
};

#endif /* __SEMAPHORE_INHERITANCE_H */
