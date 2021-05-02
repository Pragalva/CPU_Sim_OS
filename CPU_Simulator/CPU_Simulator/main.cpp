#include <iostream>
#include <string>
#include "Simulator.h"
#include "RandomAssign.h"

int main() {
	int hprocess, lprocess, runtime, programRate;
	std::string searchName = "test";
	std::string askOutput;
	bool output = false;

	std::cout << "Please enter the amount of program per hour[program/hour]: ";
	std::cin >> programRate;
	std::cout << "Please enter the amount of high priority processors: ";
	std::cin >> hprocess;
	std::cout << "Please enter the amount of low priority processors: ";
	std::cin >> lprocess;
	std::cout << "Please enter the total simulation time [hours]: ";
	std::cin >> runtime;
	runtime *= 60;

	std::cout << "Text Output?[Y]: ";
	std::cin >> askOutput;
	if (askOutput == "Y" || askOutput == "y")
	{
		output = true;
	}

	Simulator sim(hprocess, lprocess, runtime, programRate, output);
	sim.run();
	sim.report();

	while (searchName != "n")
	{
		std::cout << "Program search(input 'n' to exit or 'all' to list all programs that ran): ";
		std::cin >> searchName;
		sim.nameToSearch(searchName);
	}
	return 0;
}