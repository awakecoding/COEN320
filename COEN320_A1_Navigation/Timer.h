
#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

class Timer
{
public:
	Timer(struct sigevent* event);
	virtual ~Timer();

	void SetTime(long sec, long nsec);

private:
	timer_t timer_id;
	struct sigevent* event;
	struct itimerspec timerspec;

	void SetValue(long sec, long nsec);
	void SetInterval(long sec, long nsec);
};

#endif /* TIMER_H_ */
