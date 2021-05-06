#pragma once
#include <vector>
#include "ReadyQueue.h"
#include "Record.h"

class LprocessorQueue : public ReadyQueue
{
public:
	Programs* treating = new Programs;
	ReadyQueue* Lprocessor;
	int min_run_time;
	int max_run_time;
	int run_time;
	int start_run_time = -1;
	int end_run_time = -1;
	int IO_wait = -1;
	int percent_wait = -1;
	int wait_start = -1;
	bool report_output;
	LprocessorQueue(RandomAssign* random, ReadyQueue* N,
		int min_run_time_, int max_run_time_, bool output) :
		ReadyQueue(random), Lprocessor(N),
		min_run_time(min_run_time_), max_run_time(max_run_time_), report_output(output) {}
	
	//update LProcessorQueue
	void update(int t)
	{
		if (treating->end_run_time != -1)
		{
			//Output for when the processor has finished the current program it is running
			if (treating->end_run_time == t) {
				if (report_output)
				{
					std::cout << "Lprocessor finished run for " << treating->program_name << " with priority " << treating->priority_number << " at " << t << std::endl;
				}
				//Collect statistics
				Record* pushRecord = new Record;
				Lprocessor->completedPrograms.push_back(*treating);
				pushRecord->update_report(treating);

				treating->end_run_time = -1;

				//Checks the waiting queue to see if any program can be placed back into the ready queue
				if (Lprocessor->waitqueue.empty() == false)
				{
					for (int i = 0; i < Lprocessor->waitqueue.size(); i++)
					{
						Programs* p = new Programs;
						*p = Lprocessor->waitqueue[i];
						//if any I/O wait time is completed it moves it to the ready queue
						if (p->wait_end < t)
						{

							*treating = *p;
							//update the departure event for this program
							if (treating->wait_flag == 1)
								treating->end_run_time = t + (treating->run_time - treating->percent_wait);

							else if (treating->wait_flag == 0)
								treating->end_run_time = t + (treating->wait_start);

							treating->wait_time += (t - treating->wait_end);
							std::cout << treating->program_name << " has been put back in ready queue at " << t << std::endl;
							Lprocessor->waitqueue.erase(Lprocessor->waitqueue.begin() + i);
							return;
						}
					}
				}
			}

			//Output to signify the program has gone into I/O wait
			//The current program is then removed from the current run to a wait queue 
			if ((treating->created + treating->wait_start) == t) {
				std::cout << treating->program_name << " has gone into IO wait at " << t << std::endl;
				treating->wait_flag = 1;
				Lprocessor->waitqueue.push_back(*treating);
				treating->end_run_time = -1;

			}

			if ((treating->created + treating->wait_time + 2) % 13 == 0)
			{
				std::cout << treating->program_name << " been kicked off for excceding allocated burst time " << t << std::endl;
				treating->wait_flag = 0;
				treating->wait_start = (treating->created + treating->wait_time + treating->run_time) - t;
				Lprocessor->waitqueue.push_back(*treating);
				treating->end_run_time = -1;
			}


			return;
		}

		if (lowP_Programs.empty())
		{
			ReadyQueue* Lprocessor1;
			//Check the queue if empty wait till next event
			if (Lprocessor->lowP_Programs.empty()) {
				return;
			}
			Lprocessor1 = Lprocessor;
			Programs* p = new Programs;

			//Pick the program from the top of low priority queue and create a departure event for it
			*p = Lprocessor1->lowP_Programs.front();
			Lprocessor1->lowP_Programs.pop();

			//Allocate a cpu burts time for the program
			//Burts time are randomly generated based on real world data
			//Also records the time the program got processor time
			//It also allocates when the program would end if there was no IO wait
			int runTime = max_run_time - min_run_time;
			run_time = min_run_time + pushRandom->next_int(runTime);
			start_run_time = t;
			end_run_time = t + runTime;

			//Randonly generates IO wait time
			//The program also set a time for when the program will be needing I/O from user
			IO_wait = pushRandom->rand_IO_wait();
			wait_start = pushRandom->randIntRange(run_time);

			//Assigns all the variables to the current program
			//This code makes the I/O wait event and the departure event
			p->run_time = run_time;
			p->end_run_time = end_run_time;
			p->wait_time = start_run_time - p->created;
			p->percent_wait = IO_wait;
			p->wait_start = wait_start;
			p->wait_end = t + wait_start + IO_wait;

			//std::cout << "Treatement TIME : " << p->treat_time << std::endl;
			// Outputs the statement saying the espcifics of the program
			if (report_output)
			{
				std::cout << "Lprocessor starting run for " << p->program_name << " with priority " << p->priority_number << " at " << t << " it also had a I/O wait of " << IO_wait << std::endl;
			}
			*treating = *p;
		}

		//Outputs a statement at the end of the program execution
		else if (t - start_run_time > run_time)
		{
			Programs* p = new Programs;
			*p = lowP_Programs.front();
			lowP_Programs.pop();
			if (report_output)
			{
				std::cout << "run for " << p->program_name << " finished at " << t << std::endl;
			}
		}
	}
};
