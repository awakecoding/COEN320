
#ifndef __TIMER_H
#define __TIMER_H

#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/syspage.h>
#include <sys/neutrino.h>

class Timer
{
public:
	Timer(long sec, long nsec);
	Timer(struct sigevent* event);
	virtual ~Timer();

	void Wait();
	void SetTime(long sec, long nsec);

private:
	int channel_id;
	int connection_id;
	timer_t timer_id;
	struct sigevent* event;
	struct itimerspec timerspec;

	void SetValue(long sec, long nsec);
	void SetInterval(long sec, long nsec);
	static int PulseCode;
};

#endif /* __TIMER_H */
