
#include "PriorityInversion.h"

#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sync.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <sys/syspage.h>

#include "Timer.h"
#include "SemaphoreCeiling.h"
#include "SemaphoreInheritance.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

#define PCnt 10 /* Maximum number of threads */
#define eps .005 
#define RELEASE_TIME_P1 4 
#define RELEASE_TIME_P2 2 
#define RELEASE_TIME_P3 0 
#define PRIORITY_P1	0.7
#define PRIORITY_P2	0.6
#define PRIORITY_P3	0.5

static float priority[PCnt] = { 0 }; /* priority of threads */
static int active_p = 0; /* determine the active thread that should be ran */

static void ThreadManager();

static Semaphore* s[PCnt];

static void* P1(void* arg)
{
	int cnt = 0;

	while(1)
	{
		pthread_mutex_lock(&mutex);

		do {
			pthread_cond_wait(&cond,&mutex); /* check for a message from ThreadManager */
		} while (active_p != 1); /* check the active thread */

		cout << "P1->";

		if (cnt == 1)
		{
			/* try to acquire mutex after running for 1 unit */
			cout << ".....Attempting to Lock Semaphore ..";
			s[0]->Lock(1, s);
		}
		else if (cnt == 3)
		{
			/* release mutex after running for 3 units */
			cout << ".....Unlocking Semaphore ..";
			s[0]->Unlock(1);
		}
		else if (cnt == 4)
		{
			/* finish after 4 units */
			cout << ".........P1 thread ends.........";
			priority[1] = 0; /* to remove process 1 from the queue of ThreadManager */
			pthread_mutex_unlock(&mutex);
			break;
		}
		cnt++;

		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

static void* P2(void* arg)
{
	int cnt = 0;

	while(1)
	{
		pthread_mutex_lock(&mutex);

		do {
			pthread_cond_wait(&cond,&mutex); /* check for a message from ThreadManager */
		} while (active_p != 2); /* check the active thread */

		cout << "P2->";

		if (cnt == 6)
		{
				cout << ".........P2 thread ends.........";
				priority[2] = 0; /* to remove process 2 from the queue of ThreadManager */
				pthread_mutex_unlock(&mutex);
				break;
		}
		cnt++;

		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

static void* P3(void* arg)
{
	int cnt = 0;

	while(1)
	{
		pthread_mutex_lock(&mutex);

		do {
			pthread_cond_wait(&cond,&mutex); /* check for a message from ThreadManager */
		} while (active_p != 3); /* check the active thread */

		cout << "P3->";

		if (cnt == 1) {
			cout << ".....Attempting to Lock Semaphore ..";
			s[0]->Lock(3,s);
		}
		else if (cnt == 3) {
			cout << ".....Unlocking Semaphore ..";
			s[0]->Unlock(3);
		}
		else if (cnt == 5) {
			cout << ".........P3 thread ends.........";
			priority[3] = 0; /* to remove process 3 from the queue of ThreadManager */
			pthread_mutex_unlock(&mutex);
			break;
		}
		cnt++;

		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

static void ThreadManager()
{
	int i;
	float p;

	/* determines that which thread should run */

	pthread_mutex_lock(&mutex);

	p = -1;

	for (i = 1; i < PCnt; i++)
	{
		/* find the thread with the most priority and set it as active thread */

		if (priority[i] > p)
		{
			active_p = i;
			p = priority[i];
		}
	}

	pthread_mutex_unlock(&mutex);
	pthread_cond_broadcast(&cond); /* send the signal to the threads */
}

void run_priority_inversion_scenario(bool ceiling_priority)
{
	int cnt = 0;
	Process* p[PCnt];
	pthread_t P1_ID, P2_ID, P3_ID; /* p1, p2, p3 threads */

	p[1] = new Process(1, (float*) &priority[1], PRIORITY_P1);
	p[2] = new Process(2, (float*) &priority[2], PRIORITY_P2);
	p[3] = new Process(3, (float*) &priority[3], PRIORITY_P3);

	for (cnt = 0; cnt < PCnt; cnt++)
	{
		if (ceiling_priority)
			s[cnt] = new SemaphoreCeiling();
		else
			s[cnt] = new SemaphoreInheritance();
	}
	cnt = 0;

	if (ceiling_priority)
	{
		/* set the priority ceiling of the mutexes */
		/* s[0].PC = ???; */
	}

	/* creating a periodic  timer to generate pulses every 1 sec. */
	Timer t(1,0);

	while(1)
	{
		pthread_mutex_lock(&mutex);
		/* release P1 t = 4 */
		if (cnt == RELEASE_TIME_P1) {
			priority[1] = PRIORITY_P1;
			pthread_create(&P1_ID , NULL, P1, NULL);
		}
		/* release P2 at t = 2 */
		if (cnt == RELEASE_TIME_P2) {
			priority[2] = PRIORITY_P2;
			pthread_create(&P2_ID , NULL, P2, NULL);
		}
		/* release P3 at t = 0 */
		if (cnt == RELEASE_TIME_P3) {
			priority[3] = PRIORITY_P3;
			pthread_create(&P3_ID , NULL, P3, NULL);
		}
		/* terminate the program at t = 30 */
		if (cnt == 30) {
			break;
		}
		pthread_mutex_unlock(&mutex);

		t.Wait(); /* wait for the timer pulse */

		cout << endl << "tick=" << cnt << /* ", active_p = " << active_p << */ "->";

		ThreadManager(); /* to find out and run the active thread */
		cnt++;
	}
}

