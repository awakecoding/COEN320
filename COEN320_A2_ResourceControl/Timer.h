
#ifndef __TIMER_H
#define __TIMER_H

#include <time.h>

class Timer
{
public:
	Timer(long sec, long nsec);
	Timer(struct sigevent* event);
	virtual ~Timer();

	void Wait();
	void SetTime(long sec, long nsec);

private:
	timer_t timer_id;
	struct sigevent* event;
	struct itimerspec timerspec;

	void SetValue(long sec, long nsec);
	void SetInterval(long sec, long nsec);
};

#endif /* __TIMER_H */
