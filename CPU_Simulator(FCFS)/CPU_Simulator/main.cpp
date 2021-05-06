#include <iostream>
#include <string>
#include "Simulator.h"
#include "RandomAssign.h"
#include <windows.h>


int main() {
	int hprocess, lprocess, runtime, programRate;
	std::string searchName = "test";
	std::string askOutput;
	bool output = true;

	/*std::cout << "Please enter the amount of program per hour[program/hour]: ";
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
	*/

	for (int i = 0; i < 10; i++) {

		hprocess = 2;
		lprocess = 1;
		runtime = 10;
		runtime *= 60;
		programRate = 40;
		Simulator sim(hprocess, lprocess, runtime, programRate, output);
		sim.run();
		sim.report(i);

		Sleep(1000);
		
	}
	return 0;
}