
#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdio.h>
#include <stdlib.h>

class Process
{
public:
	Process(int number, float* priority, float opriority);
	virtual ~Process();
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
