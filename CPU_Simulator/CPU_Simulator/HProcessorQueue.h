#pragma once
#include "ReadyQueue.h"
#include "Record.h"

class HProcessorQueue : public ReadyQueue
{
public:
	Programs* treating = new Programs;
	ReadyQueue* HProcessor;
	int min_run_time;
	int max_run_time;
	int run_time;
	int start_run_time = -1;
	int end_run_time = -1;
	int IO_wait = -1;
	int wait_start = -1;
	bool report_output;


	HProcessorQueue(RandomAssign* random, ReadyQueue* N,
		int min_run_time_, int max_run_time_, bool output) :
		ReadyQueue(random), HProcessor(N), min_run_time(min_run_time_),
		max_run_time(max_run_time_), report_output(output) {}

	void update(int t)
	{
		if (treating->end_run_time != -1)
		{
			if (treating->end_run_time == t) {
				if (report_output)
				{
					std::cout << "HProcessor finished run for " << treating->program_name << " with priority " << treating->priority_number << " at " << t << std::endl;
				}
				Record* pushRecord = new Record;
				pushRecord->update_report(treating);
				HProcessor->completedPrograms.push_back(*treating);
				treating->end_run_time = -1;

				if (HProcessor->waitqueue.empty() == false)
				{
					for (int i = 0; i < HProcessor->waitqueue.size(); i++)
					{
						Programs* p = new Programs;
						*p = HProcessor->waitqueue[i];
						//if any I/O wait time is completed it moves it to the ready queue
						if (p->wait_end < t)
						{
							*treating = *p;
							//update the departure event for this program
							treating->end_run_time = t + (treating->run_time - treating->wait_start);
							treating->wait_time += (t-treating->wait_end);
							std::cout << treating->program_name << " has been put back in ready queue at " << t << std::endl;
							HProcessor->waitqueue.erase(HProcessor->waitqueue.begin() + i);
							return;
						}
					}
				}
			}
			
			if ((treating->created + treating->wait_start) == t) {
				std::cout <<  treating->program_name << " has gone into IO wait at " << t << std::endl;
				HProcessor->waitqueue.push_back(*treating);
				treating->end_run_time = -1;

			}

			return;
		}

		if (highP_Programs.empty())
		{
			ReadyQueue* HProcessor1;
			if (HProcessor->highP_Programs.empty())
			{
				if (lowP_Programs.empty())
				{
					//Check the queue if empty wait till next event
					ReadyQueue* HProcessor2;
					if (HProcessor->lowP_Programs.empty())
					{
						return;
					}
					HProcessor2 = HProcessor;
					Programs* p = new Programs;


					//Pick the program from the top of low priority queue and create a departure event for it
					*p = HProcessor2->lowP_Programs.top();
					HProcessor2->lowP_Programs.pop();

					//Allocate a cpu burts time for the program
					//Burts time are randomly generated based on real world data
					//Also records the time the program got processor time
					//It also allocates when the program would end if there was no IO wait
					int runTime = max_run_time - min_run_time;
					run_time = min_run_time + pushRandom->next_int(runTime);
					start_run_time = t;
					end_run_time = t + runTime; // NEW
					
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
					if (report_output)
					{
						std::cout << "HProcessor starting run for " << p->program_name << " with priority " << p->priority_number << " at " << t << std::endl;
					}
					*treating = *p;
				}
				return;
			}
			HProcessor1 = HProcessor;
			Programs* p = new Programs;
			*p = HProcessor1->highP_Programs.top();
			HProcessor1->highP_Programs.pop();

			int runTime = max_run_time - min_run_time;
			run_time = min_run_time + pushRandom->next_int(runTime);
			start_run_time = t;
			IO_wait = pushRandom->rand_IO_wait();
			wait_start = pushRandom->randIntRange(run_time);
			end_run_time = t + runTime; // NEW

			p->run_time = run_time;
			p->end_run_time = end_run_time;
			p->wait_time = start_run_time - p->created;
			p->percent_wait = IO_wait; 
			p->wait_start = wait_start;
			p->wait_end = t + wait_start + IO_wait;
			if (report_output)
			{
				std::cout << "HProcessor starting run for " << p->program_name << " with priority " << p->priority_number << " at " << t << std::endl;
			}
			*treating = *p;

		}
		else if (run_time < end_run_time)
		{
			Programs* p = new Programs;
			*p = lowP_Programs.top();
			lowP_Programs.pop();
			if (report_output)
			{
				std::cout << "run for " << p->program_name << " finished at " << t << std::endl;
				
			}
		}

	}
};
