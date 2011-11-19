
#ifndef __SEMAPHORE_CEILING_H
#define __SEMAPHORE_CEILING_H

#include "Semaphore.h"

class SemaphoreCeiling : public Semaphore
{
public:
	SemaphoreCeiling();
	virtual ~SemaphoreCeiling();
	void Lock(int p);
	void Unlock(int p);
};

#endif /* __SEMAPHORE_CEILING_H */
