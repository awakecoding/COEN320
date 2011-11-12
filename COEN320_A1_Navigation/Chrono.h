
#ifndef CLOCK_H_
#define CLOCK_H_

#include <pthread.h>
#include <inttypes.h>

class Chrono
{
public:
	Chrono();
	virtual ~Chrono();
	void PrintEvent(char* str);

private:
	double seconds;
	uint64_t clk_s;
	uint64_t clk_e;
	uint64_t frequency;
	pthread_mutex_t mutex;
};

#endif /* CLOCK_H_ */
