/**
 * COEN 320 Assignment #1
 *
 * Marc-Andre Moreau (9347100)
 * Terri-Anne Cambridge (9373071)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/syspage.h>
#include <sys/neutrino.h>

#include "DD.h"
#include "DAP.h"
#include "Data.h"
#include "Timer.h"
#include "Chrono.h"

Chrono* chrono;

void profile()
{
	DD* dd;
	DAP* dap;
	Data* data;

	int i, k;
	double exec_time;
	double best_time;
	double worst_time;
	double control_time;
	double average_time;
	uint64_t frequency;
	uint64_t clk0_s, clk0_e;
	uint64_t clk1_s, clk1_e;

	printf("Profiling Mode\n");

	data = new Data();

	dd = new DD(data);
	dap = new DAP(data);

	k = 100000;
	frequency = SYSPAGE_ENTRY(qtime)->cycles_per_sec;

	/* Control Time */

	clk0_s = ClockCycles();
	for (i = 0; i < k; i++)
	{
		clk1_s = ClockCycles();
		/* null program */
		clk1_e = ClockCycles();
	}
	clk0_e = ClockCycles();

	control_time = (((double)(clk0_e - clk0_s)) / frequency) / k;

	/* DD Profiling */

	best_time = 10000.0f; /* large number */
	worst_time = 0.0f; /* small number */
	average_time = 0.0f;

	for (i = 0; i < k; i++)
	{
		clk1_s = ClockCycles();
		dd->Execute();
		clk1_e = ClockCycles();

		exec_time = (((double)(clk1_e - clk1_s)) / frequency) / k;

		if (exec_time > worst_time)
			worst_time = exec_time;

		if (exec_time < best_time)
			best_time = exec_time;

		average_time += exec_time;
	}

	average_time /= k;

	printf("Profiler:\n");
	printf("Execution Count: %d\n", k);
	printf("Average Control Time: %.16f seconds\n", control_time);

	printf("\nDD Profiling Results:\n");
	printf("BCET: %.16f seconds\n", best_time);
	printf("WCET: %.16f seconds\n", worst_time);
	printf("Average: %.16f seconds\n", average_time);

	/* DAP Profiling */

	best_time = 10000.0f; /* large number */
	worst_time = 0.0f; /* small number */
	average_time = 0.0f;

	for (i = 0; i < k; i++)
	{
		clk1_s = ClockCycles();
		dap->Execute();
		clk1_e = ClockCycles();

		exec_time = (((double)(clk1_e - clk1_s)) / frequency) / k;

		if (exec_time > worst_time)
			worst_time = exec_time;

		if (exec_time < best_time)
			best_time = exec_time;

		average_time += exec_time;
	}

	average_time /= k;

	printf("\nDAP Profiling Results:\n");
	printf("BCET: %.16f seconds\n", best_time);
	printf("WCET: %.16f seconds\n", worst_time);
	printf("Average: %.16f seconds\n", average_time);
}

int main(int argc, char *argv[])
{
	DD* dd;
	DAP* dap;
	Data* data;
	Timer* dd_timer;
	Timer* dap_timer;

	if (argc > 1)
	{
		if (strcmp(argv[1], "profile") == 0)
		{
			profile();
			return 0;
		}
	}

	data = new Data();

	dd = new DD(data);
	dap = new DAP(data);

	chrono = new Chrono();
	chrono->PrintEvent((char*) "Program Start");

	dd->StartThread();
	dap->StartThread();

	dd_timer = dd->GetPulseTimer();
	dap_timer = dap->GetPulseTimer();

	dd_timer->SetTime(6, 0);
	dap_timer->SetTime(2, 0);

	dd->JoinThread();
	dap->JoinThread();
	chrono->PrintEvent((char*) "Program End");

	delete dd;
	delete dap;
	delete data;
	delete chrono;

	return EXIT_SUCCESS;
}
