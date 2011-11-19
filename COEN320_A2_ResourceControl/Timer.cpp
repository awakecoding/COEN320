
#include "Timer.h"

int Timer::PulseCode = _PULSE_CODE_MINAVAIL;

Timer::Timer(long sec, long nsec)
{
	event = (struct sigevent*) calloc(1, sizeof(struct sigevent));
	channel_id = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
	connection_id = ConnectAttach(0, 0, channel_id, 0, 0);
	SIGEV_PULSE_INIT(event, connection_id, SIGEV_PULSE_PRIO_INHERIT, PulseCode++, NULL);
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
	timer_delete(timer_id);
}

void Timer::Wait()
{
	int status;
	char msg[64];

	status = MsgReceive(channel_id, &msg, sizeof(msg), NULL);

	return;
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
