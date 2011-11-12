
#include "DAP.h"
#include "Chrono.h"

extern Chrono* chrono;
static const double t = 2.0;
static const double g = 9.8;

DAP::DAP(Data* data)
{
	this->data = data;
}

DAP::~DAP()
{

}

void DAP::Execute()
{
	int i;
	double ax, ay, az;
	Accelerometer* accelerometer;

	accelerometer = Accelerometer::GetInstance();

	ax = accelerometer->GetX();
	ay = accelerometer->GetY();
	az = accelerometer->GetZ();

	data->Lock();

	data->vx = data->vx + ax * t;
	data->vy = data->vy + ay * t;
	data->vz = data->vz + (az - g) * t;
	data->x = data->x + data->vx * t;
	data->y = data->y + data->vy * t;
	data->z = data->z + data->vz * t;

	/* waste time */
	for (i = 0; i < 100000; i++)
	{
		i--;
		i++;
	}

	data->Unlock();
}

void DAP::ThreadFunction()
{
	int status;
	char msg[64];

	channel_id = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);

	chrono->PrintEvent((char*) "DAP Thread Started");

	while (1)
	{
		status = MsgReceive(channel_id, &msg, sizeof(msg), NULL);

		if (status != 0)
			continue;

		chrono->PrintEvent((char*) "DAP Pulse Signal");

		Execute();
	}

	pthread_exit(NULL);
}

