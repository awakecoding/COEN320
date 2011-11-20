
#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define PRIORITY_COMPLETED	0
#define PRIORITY_SUSPENDED	-1

class Process
{
public:
	Process(int number, float* priority, float opriority);
	virtual ~Process();
	void Suspend();
	void Resume();;
	bool IsSuspended();
	void SetPriority(float priority);
	float GetPriority();
	float GetOriginalPriority();
	int GetNumber();
	bool IsBlocked();
	Process* GetBlockingProcess();
	void SetBlockingProcess(Process* process);
	static int GetCount();
	static Process* GetProcess(int p);
	static void SetTable(Process** table, int count);
private:
	int number;
	int suspend_s;
	float* priority;
	float opriority;
	Process* blocking_p;
	static int count;
	static Process** table;
};

#endif /* __PROCESS_H */
