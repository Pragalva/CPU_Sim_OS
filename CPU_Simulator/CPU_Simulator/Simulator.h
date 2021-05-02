#pragma once
#include <vector>
#include "Scheduler.h"
#include "HProcessorQueue.h"
#include "LprocessorQueue.h"
#include "RandomAssign.h"
#include "Record.h"

class Simulator
{
private:
	int runtime;
	std::vector<ReadyQueue*> queues;
	RandomAssign* random;
	Schedule* nameSearch;
	bool report_output;

public:
	//Creators for the High priority Processor and low Priority Processor
	//Max number will be 2 for both processors
	//Pushes all the created processor are then pushed to a queue in scheduler 
	Simulator(int HProcessors, int Lprocessors, int sim_duration, int patient_income_rate, bool report) :
		runtime(sim_duration), report_output(report)
	{
		random = new RandomAssign;
		Schedule* reception = new Schedule(random, patient_income_rate, "Reception Queue", report_output);
		nameSearch = reception;
		queues.push_back(reception);
		if (Lprocessors == 1) {
			LprocessorQueue* Lprocessor1 = new LprocessorQueue(random, reception, 1, 10, report_output);
			queues.push_back(Lprocessor1);
		}
		if (Lprocessors == 2) {
			LprocessorQueue* Lprocessor1 = new LprocessorQueue(random, reception, 1, 10, report_output);
			LprocessorQueue* Lprocessor2 = new LprocessorQueue(random, reception, 1, 10, report_output);
			queues.push_back(Lprocessor1);
			queues.push_back(Lprocessor2);
		}
		if (HProcessors == 1) {
			HProcessorQueue* doc1 = new HProcessorQueue(random, reception, 1, 20, report_output);
			queues.push_back(doc1);
		}
		if (HProcessors == 2) {
			HProcessorQueue* doc1 = new HProcessorQueue(random, reception, 1, 20, report_output);
			HProcessorQueue* doc2 = new HProcessorQueue(random, reception, 1, 20, report_output);
			queues.push_back(doc1);
			queues.push_back(doc2);
		}
	}

	//Main runner function that simulates
	// calls the update function for each processor
	void run()
	{
		for (int i = 0; i < runtime; ++i)
		{
			for (unsigned int j = 0; j < queues.size(); ++j)
				queues[j]->update(i);
		}
	}

	//Prints reports
	void report()
	{
		Record* output = new Record;
		output->print_report();
	}

	//allows the user to search for specific PID
	void nameToSearch(std::string name)
	{
		if (name != "all")
		{
			nameSearch->programSearch(name);
		}
		else
		{
			nameSearch->listAll();
		}
	}

};
