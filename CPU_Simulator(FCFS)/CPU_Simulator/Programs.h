#ifndef _PROGRAMS_H_
#define _PROGRAMS_H_

#include"RandomAssign.h"
#include<string>

class Programs
{
public:
	RandomAssign* temp;
	static int programs_count;
	std::string program_name;
	int created;
	int wait_time;
	int run_time;
	int priority_number;
	int end_run_time;
	int percent_wait;
	int wait_start;
	int wait_end;
	int wait_flag;

	Programs()
	{
		program_name = "";
		created = -1;
		wait_time = -1;
		run_time = -1;
		priority_number = -1;
		end_run_time = -1;
		percent_wait = -1;
		wait_start = -1;
		wait_end = -1;
		wait_flag = -1;
	}

	Programs(int t, int priority)
	{
		program_name = "";
		created = t;
		wait_time = 0;
		run_time = 0;
		priority_number = priority;
		wait_start = 0;
		wait_end = 0;
		wait_flag = -1;
		programs_count++;
	};

	bool operator<(const Programs& other) const
	{
		return priority_number < other.priority_number;
	}
};

int Programs::programs_count = 0;
#endif
