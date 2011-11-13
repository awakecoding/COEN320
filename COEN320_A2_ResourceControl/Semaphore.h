
#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

class Semaphore
{
public:
	float pc;
	Semaphore();
	virtual ~Semaphore();
	virtual void Lock(int p, Semaphore* s[]);
	virtual void Unlock(int p);
};

#endif /* __SEMAPHORE_H */
