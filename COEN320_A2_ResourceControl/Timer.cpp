
#include "Timer.h"

Timer::Timer(long sec, long nsec)
{
	this->event = event;
	timer_create(CLOCK_REALTIME, event, &timer_id);
	SetTime(sec, nsec);
}

Timer::Timer(struct sigevent* event)
{
	this->event = event;
	timer_create(CLOCK_REALTIME, event, &timer_id);
}

Timer::~Timer()
{

}

void Timer::Wait()
{
	/* wait for timer tick */
}

void Timer::SetTime(long sec, long nsec)
{
	SetValue(sec, nsec);
	SetInterval(sec, nsec);
	timer_settime(timer_id, 0, &timerspec, NULL);
}

void Timer::SetValue(long sec, long nsec)
{
	timerspec.it_value.tv_sec = sec;
	timerspec.it_value.tv_nsec = nsec;
}

void Timer::SetInterval(long sec, long nsec)
{
	timerspec.it_interval.tv_sec = sec;
	timerspec.it_interval.tv_nsec = nsec;
}
