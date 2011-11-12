
#ifndef DAP_H_
#define DAP_H_

#include "Data.h"
#include "Thread.h"
#include "Accelerometer.h"

class DAP : public Thread
{
public:
	DAP(Data* data);
	virtual ~DAP();
	void Execute();
	void ThreadFunction();
private:
	Data* data;
};

#endif /* DAP_H_ */
