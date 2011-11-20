
#include "Process.h"

int Process::count = 0;
Process** Process::table;

Process::Process(int number, float* priority, float opriority)
{
	this->number = number;
	this->priority = priority;
	this->opriority = opriority;
	this->blocking_p = NULL;
}

Process::~Process()
{

}

void Process::Suspend()
{
	*(priority) = PRIORITY_SUSPENDED;
}

bool Process::IsSuspended()
{
	return (*priority == PRIORITY_SUSPENDED);
}

void Process::Resume()
{
	*priority = opriority;
}

void Process::SetPriority(float priority)
{
	*(this->priority) = priority;
}

float Process::GetPriority()
{
	return *(this->priority);
}

float Process::GetOriginalPriority()
{
	return opriority;
}

int Process::GetNumber()
{
	return number;
}

bool Process::IsBlocked()
{
	return (this->blocking_p != NULL);
}

Process* Process::GetBlockingProcess()
{
	return this->blocking_p;
}

void Process::SetBlockingProcess(Process* process)
{
	this->blocking_p = process;
}

int Process::GetCount()
{
	return Process::count;
}

void Process::SetTable(Process** table, int count)
{
	Process::table = table;
	Process::count = count;
}

Process* Process::GetProcess(int p)
{
	return Process::table[p];
}
