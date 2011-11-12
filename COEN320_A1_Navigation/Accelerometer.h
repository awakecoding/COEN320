
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdio.h>
#include <stdlib.h>

class Accelerometer
{
public:
	double GetX();
	double GetY();
	double GetZ();

	static Accelerometer* GetInstance();

private:
	double x, y, z;
	Accelerometer();
	virtual ~Accelerometer();
	static Accelerometer* instance;
};

#endif /* ACCELEROMETER_H_ */
