
#include "SemaphoreInheritance.h"

SemaphoreInheritance::SemaphoreInheritance()
{
	locking_process = -1;
	locked_process = -1;
	pthread_mutex_init(&mutex, NULL);
}

SemaphoreInheritance::~SemaphoreInheritance()
{
	pthread_mutex_destroy(&mutex);
}

void SemaphoreInheritance::Lock(int p, Semaphore* s[])
{
	if (pthread_mutex_trylock(&mutex) == 0)
	{
		locking_process = p;
	}
	else
	{
		/* mutex already locked! */

		cout << ", already owned by P" << locking_process;

		Process* plocked = Process::GetProcess(p);
		Process* plocking = Process::GetProcess(locking_process);

		if (plocking->IsSuspended())
		{
			cout << ", P" << locking_process << " is already suspended, deadlock!";
			plocked->Suspend();
		}
		else
		{
			cout << ", suspending P" << p;
			plocking->SetPriority(plocked->GetPriority());
			plocked->Suspend();
			locked_process = p;
		}
	}
}

void SemaphoreInheritance::Unlock(int p)
{
	float priority;
	float opriority;

	Process* process = Process::GetProcess(p);

	priority = process->GetPriority();
	opriority = process->GetOriginalPriority();

	if (priority != opriority)
	{
		/* restore priority */
		process->SetPriority(opriority);

		/* unlock semaphore */
		pthread_mutex_unlock(&mutex);
		locking_process = -1;

		Process* locked_p = Process::GetProcess(locked_process);
		cout << "..resuming P" << locked_process << "..";
		locked_p->Resume();

		/* lock semaphore for locked process */
		this->Lock(locked_process, NULL);
		locked_process = -1;
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		locking_process = -1;
	}
}
