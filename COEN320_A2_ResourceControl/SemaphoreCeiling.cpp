
#include "SemaphoreCeiling.h"

SemaphoreCeiling::SemaphoreCeiling()
{
	this->number = number;
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
			process->SetBlockingProcess(plocking);
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
	float priority;
	float opriority;

	Process* process = Process::GetProcess(p);

	priority = process->GetPriority();
	opriority = process->GetOriginalPriority();

	if (priority != opriority)
	{
		int s;
		int k;
		Process* proc;
		Semaphore* sem;
		bool blocking = false;

		pthread_mutex_unlock(&mutex);
		locking_process = -1;
		locked_process = -1;

		for (k = 1; k <= Process::GetCount(); k++)
		{
			proc = Process::GetProcess(k);

			if (proc->IsBlocked())
			{
				if (proc->GetBlockingProcess()->GetNumber() == p)
				{
					for (s = 1; s <= Semaphore::count; s++)
					{
						sem = Semaphore::GetSemaphore(s);

						if (sem->locking_process == p)
						{
							/* this is another semaphore locked by the invoking process */
							blocking = true;
						}
					}

					if (!blocking)
					{
						process->SetPriority(opriority);
						proc->SetBlockingProcess(NULL);

						cout << ", resuming P" << proc->GetNumber() << "";
						proc->Resume();
					}
				}
			}
		}
	}
	else
	{
		pthread_mutex_unlock(&mutex);
		locking_process = -1;
	}
}
