#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include "ReadyQueue.h"

class Sechdule : public ReadyQueue
{
private:
	int priority;
	int p_influx;
	int progam_Counter = 0;
	std::string p_name;
	bool report_output;
	RandomAssign* pushRandom = new RandomAssign;
	std::set<std::string> names;
public:
	Sechdule(RandomAssign* r, int influx_rate, std::string pname, bool output) :
		ReadyQueue(r), p_influx(influx_rate), p_name(pname), report_output(output)
	{
		pushRandom = r;
		nameInput();
	}

	void update(int t)
	{
		if (t % 60 == 0)
		{
			progam_Counter = p_influx;
		}
		if (program_Counter != 0)
		{
			priority = pushRandom->rand_priority();
			Programs* program = new Programs(t, priority);
			program->program_name = setRandomName();
			if (priority <= 10) {
				lowP_programs.push(*program);
			}
			else
			{
				highP_programs.push(*program);
			}
			if (report_output)
			{
				std::cout << "Program " << program->program_name << " has been created with a priority of "
					<< program->priority_number
					<< " at " << t << std::endl;
			}
			program_Counter--;
		}
	}

	void nameInput()
	{
		std::ifstream programs;
		programs.open("patients.txt");
		std::string tempName;
		while (programs.good()) {
			programs >> tempName;
			names.insert(tempName);
		}
	}

	std::string setRandomName() {
		std::set<std::string>::iterator it = names.begin();
		int randTemp = pushRandom->randIntRange(names.size());
		for (int i = 0; i < randTemp; ++i)
		{
			++it;
		}
		return *it;
	}
};
