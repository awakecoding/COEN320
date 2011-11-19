
#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdio.h>
#include <stdlib.h>

#define PRIORITY_COMPLETED	0
#define PRIORITY_SUSPENDED	-1

class Process
{
public:
	Process(int number, float* priority, float opriority);
	virtual ~Process();
	void Suspend();
	void Resume();
	bool IsSuspended();
	void SetPriority(float priority);
	float GetPriority();
	float GetOriginalPriority();
	int GetNumber();
	static int GetCount();
	static Process* GetProcess(int p);
private:
	int number;
	float* priority;
	float opriority;
	static int count;
	static Process** table;
};

#endif /* __PROCESS_H */
