
#include "Process.h"

int Process::count = 0;
Process** Process::table = NULL;

Process::Process(int number, float* priority, float opriority)
{
	this->number = number;
	this->priority = priority;
	this->opriority = opriority;
	count++;

	if (table == NULL)
		table = (Process**) malloc(sizeof(Process*) * (count + 1));
	else
		table = (Process**) realloc(table, sizeof(Process*) * (count + 1));

	table[number] = this;
}

Process::~Process()
{

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
	return this->opriority;
}

int Process::GetNumber()
{
	return number;
}

int Process::GetCount()
{
	return count;
}

Process* Process::GetProcess(int p)
{
	return table[p];
}
