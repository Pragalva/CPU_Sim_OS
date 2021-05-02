#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include "ReadyQueue.h"

class Schedule : public ReadyQueue
{
private:
	//int wait;
	int priority;
	int p_influx;
	int programCounter = 0;
	std::string p_name;
	bool report_output;
	RandomAssign* pushRandom = new RandomAssign;
	std::set<std::string> names;
public:
	Schedule(RandomAssign* r, int influx_rate, std::string pname, bool output) :
		ReadyQueue(r), p_influx(influx_rate), p_name(pname), report_output(output)
	{
		pushRandom = r;
		nameInput();
	}

	//update program 
	//Takes the avarage rate of event creation. We made this a variable so that we can check performance in different program volume 
	void update(int t)
	{
		if (t % 60 == 0)
		{
			programCounter = p_influx;
		}
		//checks the programcounter, when !0 then gets a random prog. name and assisns a priority to it.
		if (programCounter != 0)
		{
			//Randomly generates a priority for the program
			priority = pushRandom->rand_priority();
			//wait = pushRandom->rand_IO_wait();
			
			//Creates a program with a random PID and a random priorioty
			//At this point only the priority, PID and the arrival evenet is scheduled
			Programs* program = new Programs(t, priority);
			program->program_name = setRandomName();

			//Based on the prority the program is send to the specific queue
			if (priority <= 10) {
				lowP_Programs.push(*program);
			}
			else
			{
				highP_Programs.push(*program);
			}
			if (report_output)
			{
				std::cout << program->program_name << " has checked in with a priority of "
					<< program->priority_number
					<< " at " << t << std::endl;
			}
			programCounter--;
		}
	}

	//gets the names for the input and stores in a vector of strings
	void nameInput()
	{
		std::ifstream programs;
		programs.open("numbers.txt");
		std::string tempName;
		while (programs.good()) {
			programs >> tempName;
			names.insert(tempName);
		}
	}

	//picks a ramdom rame from the list of random PID
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
