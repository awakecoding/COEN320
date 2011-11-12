
#include "Thread.h"

#include <inttypes.h>
#include <sys/syspage.h>
#include <sys/neutrino.h>

int Thread::PulseCode = _PULSE_CODE_MINAVAIL;

void* EntryPoint(void* data);

Thread::Thread()
{
	event = NULL;
	connection_id = 0;
	pulse_timer = NULL;
}

Thread::~Thread()
{
	if (event != NULL)
		free(event);
}

void Thread::StartThread()
{
	pthread_create(&thread_id, NULL, EntryPoint, (void*) this);
}

void Thread::JoinThread()
{
	void* status;
	pthread_join(thread_id, &status);
}

pthread_t Thread::GetThreadId()
{
	return thread_id;
}

int Thread::GetChannelId()
{
	return channel_id;
}

int Thread::GetConnectionId()
{
	/* this is meant to be called from another thread */

	if (connection_id == 0)
		connection_id = ConnectAttach(0, 0, this->GetChannelId(), 0, 0);

	return connection_id;
}

struct sigevent* Thread::GetSignalEvent()
{
	/* this is meant to be called from another thread */

	if (event == NULL)
	{
		event = (struct sigevent*) malloc(sizeof(struct sigevent));
		SIGEV_PULSE_INIT(event, GetConnectionId(), SIGEV_PULSE_PRIO_INHERIT, PulseCode++, NULL);
	}

	return event;
}

Timer* Thread::GetPulseTimer()
{
	/* this is meant to be called from another thread */

	if (pulse_timer == NULL)
		pulse_timer = new Timer(GetSignalEvent());

	return pulse_timer;
}

void* EntryPoint(void* data)
{
	Thread* thread = static_cast<Thread*>(data);
	thread->ThreadFunction();
	pthread_exit(NULL);
}
