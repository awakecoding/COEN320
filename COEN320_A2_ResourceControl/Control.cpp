
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Deadlock.h"
#include "PriorityInversion.h"

bool deadlock_scenario = false;
bool inversion_scenario = false;
bool ceiling_priority = false;
bool inheritance_priority = false;

int main(int argc, char* argv[])
{
	int index = 1;

	if (argc < 2)
	{
		printf("Usage:\n");
		printf("./resource_control --scenario <scenario> --priority <priority>\n");
		printf("--scenario: deadlock, inversion\n");
		printf("--priority: ceiling, inheritance\n");

		return 0;
	}

	while (index < argc)
	{
		if (strcmp(argv[index], "--scenario") == 0)
		{
			index++;

			if (strcmp(argv[index], "deadlock") == 0)
				deadlock_scenario = true;
			else if (strcmp(argv[index], "inversion") == 0)
				inversion_scenario = true;
			else
			{
				printf("invalid scenario: %s\n", argv[index]);
				return 0;
			}

			printf("scenario: %s\n", argv[index]);
		}
		else if (strcmp(argv[index], "--priority") == 0)
		{
			index++;

			if (strcmp(argv[index], "ceiling") == 0)
				ceiling_priority = true;
			else if (strcmp(argv[index], "inheritance") == 0)
				inheritance_priority = true;
			else
			{
				printf("invalid priority: %s\n", argv[index]);
				return 0;
			}

			printf("priority: %s\n", argv[index]);
		}

		index++;
	}

	if (inversion_scenario)
		run_priority_inversion_scenario(ceiling_priority);
	else if (deadlock_scenario)
		run_deadlock_scenario(ceiling_priority);

	return 0;
}


