#include <iostream>
#include <string.h>
#include <vector>
#include <cstring>
#include <cctype>
#include <chrono>
#include <ctime>
#include "validate.h"
#include "database.h"
#include "user.h"
#include "cache.h"
#include <iomanip>
#include <sstream>

void sayHello(std::string time);
std::string getTimeOfDay();
std::string capitalizeFIrstLetter(std::string name);
std::string getInputWithSpaces(const std::string& prompt);
std::string convertToMySQLDatetime(const std::string& date, const std::string& time);
void addTask(std::string user_id);
void getCurrentDataUser(const std::string username, USER &curentUser, std::vector<USER> userData);
void displayData();
bool checkLogin(std::string username, std::string password);
void loadTaskData();
void menu(std::string username);
void logIn();
void signIn();

// funcion main
int main() {
	getMysqlConnection();
	int choise;
	if (conn) {
		if(checkCache()) {
			std::string username = getUsernameDataInCacheFile();
			menu(username);
		} else {
			std::cout << "Welcome to Task Manager" << std::endl;
			std::cout << "1. Log in\n2. Sign in" << std::endl;
			std::cout << "Choise = " ; 
			std::cin >> choise;
			
			switch(choise) {
				case 1:
					logIn();
					break;
				case 2:
					signIn();
					break;
				case 3:
					std::cout << "data data" << std::endl;		
				default:
					break;
			}
		}
		
	
		
	} else {
		std::cout << "Koneksi Database Eror" << std::endl;
	}
	return 0;
}

// function untuk memberikan ucapan kepada user sesuai waktu real time
void sayHello(std::string time) {
	std::cout << "Selamat " << time <<" "<<  curentUser.name << std::endl;
}

// function untuk mendapatkan waktu real time 
std::string getTimeOfDay() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);
    int hour = localTime->tm_hour;
    if (hour >= 5 && hour < 12) {
        return "Pagi";
    } else if (hour >= 12 && hour < 15) {
        return "Siang";
    } else if (hour >= 15 && hour < 18) {
        return "Sore";
    } else {
        return "Malam";
    }
}

// funtion yang digunakan untuk mengubah huruf pertama pada kata menjadi huruf kapital
std::string capitalizeFIrstLetter(std::string name) {
	int jumlah_huruf = name.length();
	bool setelah_spasi = true;
	for (int i = 0; i < jumlah_huruf; i++) {
		if (name[i] == ' ') {
			setelah_spasi = true;
			continue;
		} 
		if (setelah_spasi) {
			if (name[i] >= 'a' && name[i] <= 'z' ) {
      	name[i] = name[i] - 32;
			} 
        setelah_spasi = false;
   	 } 
		else if (!setelah_spasi) {
		 if (name[i] >= 'A' && name[i] <= 'Z') {
        name[i] = name[i] + 32;
        setelah_spasi = false;
			}			
		}
	}
	return name;
}

// funtion untuk input lebih dari 1 kata
std::string getInputWithSpaces(const std::string& prompt) {
	std::cin.ignore();
	std::string input;
	std::cout << prompt;
	getline(std::cin, input);
	return input;
}

// funtion untuk mengubah string date dan time menjadi format string mysql
std::string convertToMySQLDatetime(const std::string& date, const std::string& time) {
    // Asumsi input date dalam format DD-MM-YYYY dan input time dalam format HH:MM
    int day, month, year, hour, minute;
    char dash1, dash2, colon;
    std::istringstream date_ss(date);
    std::istringstream time_ss(time);

    date_ss >> day >> dash1 >> month >> dash2 >> year;
    time_ss >> hour >> colon >> minute;

    std::ostringstream datetime_ss;
    datetime_ss << std::setw(4) << std::setfill('0') << year << "-"
                << std::setw(2) << std::setfill('0') << month << "-"
                << std::setw(2) << std::setfill('0') << day << " "
                << std::setw(2) << std::setfill('0') << hour << ":"
                << std::setw(2) << std::setfill('0') << minute << ":00";
    return datetime_ss.str();
}

// function untuk menambah tugas yang dimana tugas tersebut akan di simpan ke database
void addTask(std::string user_id) {
    std::string task_name = getInputWithSpaces("Masukkan task name: ");
    std::string description = getInputWithSpaces("Masukkan description: ");
    std::string date = getInputWithSpaces("Masukkan tanggal (DD-MM-YYYY): ");
    std::string time = getInputWithSpaces("Masukkan jam (HH:MM): ");
    std::string datetime = convertToMySQLDatetime(date, time);

    addTaskData(task_name, description, datetime, user_id);
    std::cout << "Task berhasil ditambahkan!" << std::endl;
}

// funtion untuk mendapatkan data lengkap user yang sedang log in
// function berfungdi mengisi varibel struct current user
void getCurrentDataUser(const std::string username, USER &curentUser, std::vector<USER> userData) {
	for (int i = 0; i < userData.size(); i++) {
		if (userData[i].username == username) {
			curentUser.id = userData[i].id;
			curentUser.username = userData[i].username;
			curentUser.password = userData[i].password;
			curentUser.created_at = userData[i].created_at;
		}
	}
}

void displayData() {
	for (int i = 0; i < userData.size(); i++) {
    std::cout << "id " << userData[i].id  << std::endl;
    std::cout << "username : " << userData[i].username << std::endl;
    std::cout << "password : " << userData[i].password << std::endl;
    std::cout << "create : "<< userData[i].created_at << std::endl;;
  }
}

// function untuk cek apakah username dan password yang diinputkan saat login ada atau tidak
bool  checkLogin(std::string username, std::string password) {
	for (int i = 0; i < userData.size(); i++) {
		if (username == userData[i].username && password == userData[i].password) {
			return true;
		}	
  }
  return false;
}

// function untuk memuat semua data
void loadTaskData() {
   const int numWidth = 5;
    const int nameWidth = 30;
    const int tableWidth = numWidth + nameWidth + 7; // Total lebar tabel termasuk border dan spasi

    // Garis pemisah header dan footer
    auto printSeparator = [&]() {
        std::cout << "+";
        for (int i = 0; i < numWidth + 2; ++i) std::cout << "=";
        std::cout << "+";
        for (int i = 0; i < nameWidth + 2; ++i) std::cout << "=";
        std::cout << "+" << std::endl;
    };

    // Fungsi untuk mencetak teks rata tengah
    auto printCentered = [](const std::string& text, int width) {
        int padding = width - text.length();
        int paddingLeft = padding / 2;
        int paddingRight = padding - paddingLeft;
        return std::string(paddingLeft, ' ') + text + std::string(paddingRight, ' ');
    };

    // Header tabel
    printSeparator();
    std::cout << "| " << printCentered("No.", numWidth)
              << " | " << printCentered("Task Name", nameWidth) << " |" << std::endl;
    printSeparator();

    // Isi tabel dengan border dan garis pemisah setiap baris
    for (int i = 0; i < taskData.size(); i++) {
        std::string numStr = std::to_string(i + 1);
        std::cout << "| " << printCentered(numStr, numWidth)
                  << " | " << printCentered(taskData[i].task_name, nameWidth) << " |" << std::endl;
        printSeparator();
    }
}

// funtion untuk menampilkan menu ke user
void menu(std::string username) {
	system("cls");
	sayHello(getTimeOfDay());
	int choise;
	getDataTaskByUsername(username);
	loadTaskData();
	std::cout << "2. Add Task" << std::endl;
	std::cout << "0. log out" << std::endl;
	std::cout << "choise : ";
	std::cin >> choise;
	switch (choise) {
	case  0: 
		deleteCache();
		break;
	case  2: 
		addTask("19");
		break;
	default :
		break;
	}
}

// function untuk sistem login
void logIn() {
	system("cls");
	std::string username;
	std::string password;
	std::cout << "======= Log In =======" << std::endl;
	std::cout << "Username : ";
	std::cin >> username;
	std::cout << "Password : ";
	std::cin >> password; 
	getUserData();
	if(checkLogin(username, password)) {
		getCurrentDataUser(username, curentUser, userData);
		std::cout << "masuk menu" << std::endl;
		createCache(username, password);
		menu(username);
	} else {
		logIn();
		
	}
}

// funtion untuk sistem signin
void signIn() {
	std::string name;
	std::string username;
	std::string password1;
	std::string password2;
	std::cout << "======= Sign In =======" << std::endl;
	std::cout << "Name : ";
	std::cin.ignore();
	getline(std::cin, name);
	std::cout << "Username : ";
	std::cin >> username;
	std::cout << "Password : ";
	std::cin >> password1;
	std::cout << "Password : ";
	std::cin >> password2;

	if (validateData(name, username, password1, password2)) {
		if(addDataUser(capitalizeFIrstLetter(name), username, password1)) {
			createCache(username, password1);
			menu(username);
		}
	} else {
		signIn();
	}
}
