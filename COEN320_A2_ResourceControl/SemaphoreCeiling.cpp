
#include "SemaphoreCeiling.h"

SemaphoreCeiling::SemaphoreCeiling()
{
	locking_process = -1;
	locked_process = -1;
	pthread_mutex_init(&mutex, NULL);
}

SemaphoreCeiling::~SemaphoreCeiling()
{
	pthread_mutex_destroy(&mutex);
}

void SemaphoreCeiling::Lock(int p)
{
	int s;
	Semaphore* sem;
	float priority;
	Process* process;
	bool suspend = false;
	Process* plocking = NULL;

	process = Process::GetProcess(p);
	priority = process->GetPriority();

	/**
	 * If P attempts to lock a semaphore,
	 * it will be suspended unless its priority
	 * is higher than PC(S) for all S currently
	 * locked by all tasks Q != P
	 */

	for (s = 1; s <= Semaphore::count; s++)
	{
		sem = Semaphore::GetSemaphore(s);

		if ((sem->locking_process > 0) && (sem->locking_process != p))
		{
			if (!(priority > sem->pc))
			{
				plocking = Process::GetProcess(sem->locking_process);
				suspend = true;
			}
		}
	}

	/**
	 * If P is suspended, Q holding S with
	 * highest PC is considered to be blocking P
	 */

	if (suspend)
	{
		if (plocking->IsSuspended())
		{
			cout << ", P" << locking_process << " is already suspended, deadlock!";
			process->Suspend();
		}
		else
		{
			cout << ", suspending P" << p;
			plocking->SetPriority(priority);
			process->Suspend();
			locked_process = p;
		}
	}
	else
	{
		pthread_mutex_lock(&mutex);
		locking_process = p;
	}
}

void SemaphoreCeiling::Unlock(int p)
{
	int s;
	Semaphore* sem;
	float priority;
	float opriority;
	Process* locked_p;
	bool blocked = false;

	Process* process = Process::GetProcess(p);

	priority = process->GetPriority();
	opriority = process->GetOriginalPriority();

	if (priority != opriority)
	{
		locked_p = Process::GetProcess(locked_process);

		/* unlock semaphore */
		pthread_mutex_unlock(&mutex);
		locking_process = -1;

		for (s = 1; s <= Semaphore::count; s++)
		{
			sem = Semaphore::GetSemaphore(s);

			if (sem->locking_process == p)
			{
				if (!(locked_p->GetPriority() > sem->pc))
				{
					blocked = true;
				}
			}
		}

		if (!blocked)
		{
			/* restore priority */
			process->SetPriority(opriority);

			locked_p = Process::GetProcess(locked_process);
			cout << ", resuming P" << locked_process << "";
			locked_p->Resume();

			/* lock semaphore for locked process */
			this->Lock(locked_process);
			locked_process = -1;
		}
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		locking_process = -1;
	}
}
