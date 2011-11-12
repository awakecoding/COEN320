
#ifndef THREAD_H_
#define THREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/neutrino.h>

#include "Timer.h"

class Thread
{
public:

	Thread();
	virtual ~Thread();

	void StartThread();
	void JoinThread();
	pthread_t GetThreadId();
	int GetChannelId();
	int GetConnectionId();
	struct sigevent* GetSignalEvent();
	Timer* GetPulseTimer();

	static int PulseCode;
	virtual void Execute() = 0;
	virtual void ThreadFunction() = 0;

protected:
	int channel_id;
	int connection_id;
	pthread_t thread_id;
	struct sigevent* event;
	Timer* pulse_timer;
};

#endif /* THREAD_H_ */
