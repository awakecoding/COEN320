
#include "Chrono.h"

#include <stdio.h>
#include <sys/syspage.h>
#include <sys/neutrino.h>

Chrono::Chrono()
{
	pthread_mutex_init(&mutex, NULL);
	frequency = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	clk_s = ClockCycles();
}

Chrono::~Chrono()
{
	pthread_mutex_destroy(&mutex);
}

void Chrono::PrintEvent(char* str)
{
	pthread_mutex_lock(&mutex);

	clk_e = ClockCycles();
	seconds = (((double)(clk_e - clk_s)) / frequency);
	printf("[%.16f]: %s\n", seconds, str);

	pthread_mutex_unlock(&mutex);
}
