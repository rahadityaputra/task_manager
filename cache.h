#include <fstream>
std::ofstream cacheWrite;
std::ifstream cacheRead;

// function untuk mengecek cache , apakah sebelumnya user sudah pernah login
bool checkCache() {
	std::string isCache;
	cacheRead.open("cache.txt");
	std::getline(cacheRead, isCache);
	cacheRead.close();
	if (isCache == "true") {
		return true;
	}
	return false;
}

// function untuk memebuat cache untuk menandanai bahwa user sudah login atau belum
void createCache(std::string username, std::string password) {
	cacheWrite.open("cache.txt");
	cacheWrite << "true";
	cacheWrite << "\n" << username;
	cacheWrite << "\n" << password;
	cacheWrite.close();
}

// function untuk menghapus cache ketika user log out
void deleteCache() {
	cacheWrite.open("cache.txt");
	cacheWrite << "false";
	cacheWrite.close();
}

// function untuk mendapatkan username dari user yang login
std::string getUsernameDataInCacheFile() {
	std::string username;
	cacheRead.open("cache.txt");
	cacheRead >> username;
	cacheRead >> username;
	cacheRead.close();
	return username;
}