#pragma once

#include <cstdlib>
#include <ctime>

class RandomAssign
{
public:

	RandomAssign() {
		srand((unsigned int)std::time(0));
	}

	RandomAssign(int seed)
	{
		srand(seed);
	}

	int next_int(int n)
	{
		return int(next_double() * n);
	}

	int randIntRange(int r)
	{
		return int(rand() % r);
	}

	double next_double()
	{
		return double(rand()) / RAND_MAX;
	}

	//A function that randomly generates priority
	int rand_priority()
	{
		int probability = (rand() % 10) + 1;
		if (probability >= 4)
		{
			return (rand() % 10) + 1;
		}
		if (probability >= 2)
		{
			return (rand() % 5) + 11;
		}
		if (probability == 1)
		{
			return (rand() % 5) + 16;
		}
		return 1;
	}

	//A function that ramdonly generates the wait time allocated for the program
	int rand_IO_wait()
	{
		int probability = (rand() % 10) + 1;
		if (probability > 6)
			return 0;
		if (probability > 2)
		{
			int waitstart = (rand() % 2) + 1;
			return waitstart;
		}
		else
		{
			int waitstart = (rand() % 5) + 1;
			return waitstart;
		}
	}

};
