
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
#include "Process.h"
#include "SemaphoreCeiling.h"
#include "SemaphoreInheritance.h"

#include "Deadlock.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

#define PCnt 10 /* maximum number of threads */
#define eps .005 
#define RELEASE_TIME_P1 2 
#define RELEASE_TIME_P2 0 
#define PRIORITY_P1	0.7
#define PRIORITY_P2	0.5

static float priority[PCnt] = { 0 }; /* priority of threads */
static int active_p = 0; /* determine the active threads that should be run */

static void ThreadManager(); /* this method is already implemented */

static Semaphore* s[PCnt];

static void* P1(void* arg)
{
	int cnt = 0;

	while (1)
	{
		pthread_mutex_lock(&mutex);

		do {
			pthread_cond_wait(&cond, &mutex); /* check for a message from ThreadManager */
		} while (active_p != 1); /* check the active thread */

		cout << "P1: ";

		if (cnt == 1) {
			cout << "attempting to lock S1";
			s[1]->Lock(1,s);
		}
		else if (cnt == 2) {
			cout << "attempting to lock S2";
			s[2]->Lock(1,s);
		}
		else if (cnt == 3) {
			cout << "unlocking S2";
			s[2]->Unlock(1);
		}
		else if (cnt == 4) {
			cout << "unlocking S1";
			s[1]->Unlock(1);
		}
		else if (cnt == 5) {
			cout << "thread execution completed";
			priority[1] = PRIORITY_COMPLETED; /* to remove process 1 from the queue of ThreadManager */
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
		} while (active_p != 2); /* check the active thread	*/

		cout << "P2: ";

		if (cnt == 1) {
			cout << "attempting to lock S2";
			s[2]->Lock(2,s);
		}
		else if (cnt == 2) {
			cout << "attempting to lock S1";
			s[1]->Lock(2,s);
		}
		else if (cnt == 3){
			cout << "unlocking S1";
			s[1]->Unlock(2);
		}
		else if (cnt == 4) {
			cout << "unlocking S2";
			s[2]->Unlock(2);
		}
		else if (cnt == 5)
		{
			cout << "thread execution completed";
			priority[2] = PRIORITY_COMPLETED; /* to remove process 2 from the queue of ThreadManager */
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

	/* determines which thread should run */

	pthread_mutex_lock(&mutex);

	p = -1;

	if (priority[active_p] == PRIORITY_SUSPENDED)
		active_p = -1;

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

void run_deadlock_scenario(bool ceiling_priority)
{
	int cnt = 0;
	Process* p[PCnt];
	pthread_t P1_ID, P2_ID; /* p1, p2, threads */

	p[1] = new Process(1, (float*) &priority[1], PRIORITY_P1);
	p[2] = new Process(2, (float*) &priority[2], PRIORITY_P2);

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

		/**
		 * P1: locks S1, S2
		 * P2: locks S2, S2
		 */

		s[1]->pc = PRIORITY_P1;
		s[2]->pc = PRIORITY_P1;
	}

	/* creating a periodic  timer to generate pulses every 1 sec. */
	Timer t(1, 0);

	while(1)
	{
		pthread_mutex_lock(&mutex);
		/* release P1 at t = 2 */
		if (cnt == RELEASE_TIME_P1) {
			priority[1] = PRIORITY_P1;
			pthread_create(&P1_ID , NULL, P1, NULL);
		}
		/* release P2 at t = 0 */
		if(cnt == RELEASE_TIME_P2) {
			priority[2] = PRIORITY_P2;
			pthread_create(&P2_ID , NULL, P2, NULL);
		}
		/* terminate the program at t = 30 */
		if (cnt == 30) {
			break;
		}
		pthread_mutex_unlock(&mutex);

		t.Wait(); /* wait for the timer pulse */

		cout << endl << "TICK:" << cnt << /* ", active_p = " << active_p << */ "\t";

		ThreadManager(); /* to find out and run the active thread */
		cnt++;
	}
}

