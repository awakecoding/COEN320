
#include "Accelerometer.h"

Accelerometer* Accelerometer::instance = NULL;

Accelerometer::Accelerometer()
{
	x = -2.0;
	y = 4.0;
	z = 10.5;
}

Accelerometer::~Accelerometer()
{

}

double Accelerometer::GetX()
{
	return x;
}

double Accelerometer::GetY()
{
	return y;
}

double Accelerometer::GetZ()
{
	return z;
}

Accelerometer* Accelerometer::GetInstance()
{
	if (instance == NULL)
		instance = new Accelerometer();

	return instance;
}
