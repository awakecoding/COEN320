
#ifndef DD_H_
#define DD_H_

#include "Data.h"
#include "Thread.h"

class DD : public Thread
{
public:
	DD(Data* data);
	virtual ~DD();
	void Execute();
	void ThreadFunction();
private:
	Data* data;
};

#endif /* DD_H_ */
