#pragma once
#include <queue>
#include "Programs.h"
#include "RandomAssign.h"

class ReadyQueue
{
public:
	std::queue<Programs> lowP_Programs;
	std::vector<Programs> waitqueue;
	std::vector<Programs> completedPrograms;
	RandomAssign* pushRandom;
	virtual void update(int t) = 0;
	ReadyQueue(RandomAssign* RandomAssign)
	{
		pushRandom = RandomAssign;
	}

	//Allow the user to sreach the history of a Program. It will display when the program arrived and when it departed
	void programSearch(std::string name)
	{
		bool output = false;
		for (int prog = 0; prog < completedPrograms.size(); ++prog)
		{
			if (completedPrograms[prog].program_name == name) {
				output = true;
				std::cout << name << " was created at " << completedPrograms[prog].created << " with a priority of " << completedPrograms[prog].priority_number
					<< ", had a wait time of " << completedPrograms[prog].wait_time << " minutes, and a run time of "
					<< completedPrograms[prog].run_time << " minutes." << std::endl;
			}
		}
		if (!output)
		{
			std::cout << "No record of " << name << "." << std::endl;
		}
	}

	//list the names of all the progams that were ran in ths instance.
	void listAll()
	{
		for (int prog = 0; prog < completedPrograms.size(); ++prog)
		{
			std::cout << completedPrograms[prog].program_name << std::endl;
		}
	}
};
