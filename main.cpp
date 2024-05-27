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
#include <windows.h>


void sayHello(std::string time);
std::string getTimeOfDay();
std::string capitalizeFIrstLetter(std::string name);
std::string getInputWithSpaces(const std::string& prompt);
std::string convertToMySQLDatetime(const std::string& date, const std::string& time);
void addTask(std::string user_id);
void getCurrentDataUser(const std::string username, USER &curentUser, std::vector<USER> userData);
void displayData();
void loadTaskData();
void menu(std::string username);
void menuOptios();
void logIn();
void signIn();
void logout();
void searchTasks();
void displayTaskDetail();

// funcion main
int main() {
	getMysqlConnection();
	getUserData();

	int choise;
	if (conn) {
		if(checkCache()) {
			std::string username = getUsernameDataInCacheFile();
			getCurrentDataUser(username, curentUser, userData);
			menu(username);
		} else {
			std::cout << "Welcome to Task Manager" << std::endl;
			std::cout << "1. Log in\n2. Sign in" << std::endl;
			std::cout << "Choose an option : " ; 
			std::cin >> choise;
			switch(choise) {
				case 1:
					logIn();
					break;
				case 2:
					signIn();
					break;
				default:
					break;
			}
		}
	} else {
		std::cout << "Database Connection Error" << std::endl;
	}
	return 0;
}

// funtion untuk melihat detail task
void displayTaskDetail(int number ) {
	std::cout << "Title :" <<  taskData[number].task_name << std::endl;
	std::cout << "Description :" << taskData[number].description << std::endl;
	std::cout << "Deadline  :" << taskData[number].deadline << std::endl;
	std::cout << "Created at :"<<taskData[number].created_at << std::endl;
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
}

// funtion untuk mendapatkan data lengkap user yang sedang log in
// function berfungdi mengisi varibel struct current user
void getCurrentDataUser(const std::string username, USER &curentUser, std::vector<USER> userData) {
	for (int i = 0; i < userData.size(); i++) {
		if (userData[i].username == username) {
			curentUser.id = userData[i].id;
			curentUser.username = userData[i].username;
			curentUser.password = userData[i].password;
			curentUser.name = userData[i].name;
			curentUser.created_at = userData[i].created_at;
		}
	}
}

// function untuk menampilkan data user
// function ini sepertinya tidak akan dipakai di dalam program
void displayData() {
	for (int i = 0; i < userData.size(); i++) {
    std::cout << "id " << userData[i].id  << std::endl;
    std::cout << "username : " << userData[i].username << std::endl;
    std::cout << "password : " << userData[i].password << std::endl;
    std::cout << "create : "<< userData[i].created_at << std::endl;;
  }
}

// function untuk memuat semua data task dari database ke dalam program
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
    std::cout << "| " << printCentered("No.", numWidth) << " | " << printCentered("Task Name", nameWidth) << " |" << std::endl;
    printSeparator();

    // Isi tabel dengan border dan garis pemisah setiap baris
    for (int i = 0; i < taskData.size(); i++) {
        std::string numStr = std::to_string(i + 1);
        std::cout << "| " << printCentered(numStr, numWidth) << " | " << printCentered(taskData[i].task_name, nameWidth) << " |" << std::endl;
        printSeparator();
    }
}

// funtion untuk menampilkan menu ke user
void menu(std::string username) {
	int choise;
		system("cls");
		sayHello(getTimeOfDay());
		menuOptios();
		std::cin >> choise;
		switch (choise) {
		case  1: 
			int numberTask;
			system("cls");
			// menu menampilkan semua task
			getDataTaskByUsername(username, taskData);
			loadTaskData();
			std::cout << "See task number = ";
			std::cin >> numberTask;
			displayTaskDetail(numberTask);
			break;
		case  2: 
			system("cls");
			// menu menambah task
			addTask(curentUser.id);
			break;
		case  3: 
			system("cls");
			// menu mencari task 
			searchTasks();
			break;
		case  4: 
			// menu logout dari program
			system("cls");
			logout();
			return;
			break;
		default :
			break;
		}
		system("pause");
		menu(username);
}

void menuOptios() {
	std::cout << "1. Display All Taks" << std::endl;
	std::cout << "2. Add New Task" << std::endl;
	std::cout << "3. Search Task" << std::endl;
	std::cout << "4. Logout" << std::endl;
	std::cout << "Choose an option : ";
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
	if(checkLogin(username, password)) {
		std::cout << "Login Successful....." << std::endl;
		Sleep(2000);
		getCurrentDataUser(username, curentUser, userData);
		createCache(username, password);
		menu(username);
	} else {
		std::cout << "Login Failed....." << std::endl;
		Sleep(2000);
		logIn();
	}
}

// funtion untuk sistem signin
void signIn() {
	std::string name;
	std::string username;
	std::string password1;
	std::string password2;
std::cout << "+-------------------------------------------+" << std::endl;
	std::cout << "|           ======= Sign In =======         |" << std::endl;
    std::cout << "+-------------------------------------------+" << std::endl;
	std::cout << "\n           ======= NAME =======" << std::endl;
    std::cout << "+-------------------------------------------+" << std::endl;
    std::cout << "|     Name must have 1 - 100 characters     |" << std::endl;
    std::cout << "+-------------------------------------------+" << std::endl;
	std::cout << "Name         : ";
	std::cin.ignore();
	getline(std::cin, name);
	std::cout << "\n         ======= USERNAME =======" << std::endl;
    std::cout << "+-------------------------------------------+" << std::endl;
    std::cout << "|    Username must have 4 - 12 characters   |" << std::endl;
    std::cout << "|    Username must start with a letter      |" << std::endl;
    std::cout << "+-------------------------------------------+" << std::endl;
	std::cout << "Username     : ";
	std::cin >> username;
	std::cout << "\n         ======= PASSWORD =======" << std::endl;
    std::cout << "+-------------------------------------------+" << std::endl;
    std::cout << "|  Password must contain 8 - 16 characters  |" << std::endl;
    std::cout << "|  Password must consist of a combination   |" << std::endl;
    std::cout << "|           of leters and numbers           |" << std::endl;
    std::cout << "+-------------------------------------------+" << std::endl;
	std::cout << "Password     : ";
	std::cin >> password1;
std::cout << "Confirmation : ";
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

// function untuk log out / keluar dari program
void logout() {
	deleteCache();
}

void searchTasks() {

}
