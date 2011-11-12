
#include "DD.h"
#include "Chrono.h"

extern Chrono* chrono;

DD::DD(Data* data)
{
	this->data = data;
}

DD::~DD()
{

}

void DD::Execute()
{
	data->Lock();

	printf("x:\t%f\ty:\t%f\tz:\t%f\n", data->x, data->y, data->z);
	printf("vx:\t%f\tvy:\t%f\tvz:\t%f\n", data->vx, data->vy, data->vz);

	data->Unlock();
}

void DD::ThreadFunction()
{
	int status;
	char msg[64];

	channel_id = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);

	chrono->PrintEvent((char*) "DD Thread Started");

	while (1)
	{
		status = MsgReceive(channel_id, &msg, sizeof(msg), NULL);

		if (status != 0)
			continue;

		chrono->PrintEvent((char*) "DD Pulse Signal");

		Execute();
	}
}
