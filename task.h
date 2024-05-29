#pragma once
#include <vector>
#include <iostream>
struct TASK {
	std::string id;
	std::string task_name;
	std::string description;
	std::string deadline;
	std::string created_at;
	std::string user_id;
};

extern std::vector<TASK> taskData;

