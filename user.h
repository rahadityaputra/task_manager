#pragma once
struct USER {
	std::string id;
	std::string username;
	std::string password;
	std::string name;
	std::string created_at;
};

std::vector<USER> userData;
USER curentUser;
