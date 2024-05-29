#pragma once

#include <iostream>
#include <vector>

struct USER {
	std::string id;
	std::string username;
	std::string password;
	std::string name;
	std::string created_at;
};

extern std::vector<USER> userData;
extern USER curentUser;
