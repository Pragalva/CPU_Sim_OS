#pragma once
#include <iostream>
#include "Programs.h"
#include <fstream>

class Record
{
public:
	Record() {}
	//this updates the report record, so that it can be printed.
	//the reports are collected in this way because if a processor is idle due to a lack of program it will not affect the final data
	void update_report(Programs* Program)
	{
		total_wait_time += Program->wait_time;
		total_run_time += Program->run_time;
		total_turnaround_time += (Program->end_run_time - Program->created);
		total_num_programs += 1;
		total_response_time += Program->end_run_time;
		total_wait_percentage += Program->percent_wait;
	};

	//prints application stuff into the terminal and .csv file
	void print_report()
	{	
		//output to terminal
		std::cout << "Total Programs Treated: " << total_num_programs << std::endl;
		std::cout << "Throughput: " << (double)total_run_time / total_num_programs << std::endl;
		std::cout << "Average wait time: " << (double)total_wait_time / total_num_programs << std::endl;
		std::cout << "Average turnaround time: " << (double)total_turnaround_time / total_num_programs << std::endl;
		std::cout << "Avarage response time: " << (double)total_response_time / total_num_programs << std::endl;
		std::cout << "Processor Untilization: " << (double)(1 - total_wait_percentage/total_run_time) << std::endl;

		//output to file
		std::ofstream myFile;
		myFile.open("runReport.csv");
		myFile << "total programs Treated,Throughput,average wait time,average turnaround time,average response time, processoruntilization" << std::endl;
		myFile << total_num_programs << "," << (double)total_run_time / total_num_programs << "," <<  (double)total_wait_time / total_num_programs;
		myFile << "," << (double)total_turnaround_time / total_num_programs << "," << (double)total_response_time / total_num_programs;
		myFile << "," << (double)(1 - total_wait_percentage / total_run_time) << std::endl;
		myFile.close();
	};

private:
	static int total_run_time;
	static int total_wait_time;
	static int total_num_programs;
	static int total_turnaround_time;
	static int total_response_time;
	static double total_wait_percentage;
	Programs* test = new Programs;
};

int Record::total_run_time = 0;
int Record::total_wait_time = 0;
int Record::total_num_programs = 0;
int Record::total_turnaround_time = 0;
int Record::total_response_time = 0;
double Record::total_wait_percentage = 0;
