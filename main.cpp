#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <cstring>
#include <cctype>
#include <chrono>
#include <ctime>
#include <limits>
#include <iomanip>
#include "validate.h"
#include "database.h"
#include "cache.h"
#include <iomanip>
#include <sstream>
#include <windows.h>

void sayHello(std::string time);
std::string getTimeOfDay();
std::string getCurrentDate();
std::string capitalizeFIrstLetter(std::string name);
std::string getInputWithSpaces(const std::string& prompt);
std::string convertToMySQLDatetime(const std::string& date, const std::string& time);
void addTask(std::string user_id);
void getCurrentDataUser(const std::string username, USER &curentUser, std::vector<USER> userData);
void seeAllTaskData();
void menu();
void menuOptios(int &choise);
void logIn();
void signIn();
void logout();
void getValidatedInput(std::string &variabel, std::string prompt);
void showLoginMenu();
void deletaTasks();
bool displayAllTasksData();
void finishTask(int numberTask);
void 	menuOnTaskDetail(int numberTask);
void updateTask(int choice, int index, std::vector<TASK> taskData);


// funcion main
int main() {
	getMysqlConnection();
	if (conn) {
		getUserData();
		if(checkCache()) {
			std::string username = getUsernameDataInCacheFile();
			getCurrentDataUser(username, curentUser, userData);
			menu();
		} else {
			showLoginMenu();
		}
	} else {
		std::cout << "Database Connection Error !" << std::endl;
	}

	return 0;
}

void updateTask(int choice, int index, std::vector<TASK> taskData) {
	std::string attributeChoice;
	std::string editedValue;
	std::string date, time;
	switch (choice) {
	case 1 : 
		// update title task
		attributeChoice = "task_name";
		std::cout << "Title : "<< taskData[index].task_name << std::endl;
		editedValue = getInputWithSpaces("Edit Title : ");	
		break;
	case 2 : 
		// update description task
		attributeChoice = "description";
		std::cout << "Description : "<< taskData[index].description << std::endl;
		editedValue = getInputWithSpaces("Edit Description : ");
		break;
	case 3 : 
		// update deadline  task
		attributeChoice = "deadline";
		std::cout << "Deadline : "<< taskData[index].deadline << std::endl;
		date = getSingleWordInput("Edit date : ");
		time = getSingleWordInput("Edit time : ");
		editedValue = convertToMySQLDatetime(date, time);
		break;
	default :
		break;		
	}

	updateTaskData(editedValue, attributeChoice, index, taskData);
}

void 	menuOnTaskDetail(int numberTask) {
	int choice;
	std::cout << "1. Finish Task " << std::endl;
	std::cout << "2. Update Task " << std::endl;
	std::cout << "3. Back " << std::endl;
	choice = getValidatedInput<int>("Choose : ");
	switch (choice) {
	case  1: 
		finishTask(numberTask);			
	break;
	case  2: 
		std::cout << "1. Title " << std::endl;
		std::cout << "2. Description " << std::endl;
		std::cout << "1. Deadline " << std::endl;
		choice = getValidatedInput<int>("Choose : ");
		updateTask(choice, numberTask - 1, taskData);
		break;
	case 3:
		return;
		break;
	default :
		break;
	}
}

std::string getCurrentDate() {
    // Dapatkan waktu saat ini
    std::time_t t = std::time(nullptr);
    // Konversi ke struct tm
    std::tm* now = std::localtime(&t);

    // Gunakan stringstream untuk format output
    std::ostringstream oss;
    oss << std::put_time(now, "%d-%m-%Y");

    // Kembalikan string hasil format
    return oss.str();
}

// funtion untuk finish task
void finishTask(int  numberTask) {
		deleteTasksData(taskData[numberTask - 1].id);
}

// funtion menu delete task
void deleteTasks () {
	int deleteNumber;
	if (displayAllTasksData()) {
		deleteNumber = getValidatedInput<int>("The option number of the task you want to delete :  ");
		deleteTasksData(taskData[deleteNumber - 1].id);
	}
}

// funtion untuk menampilkan menu login dan sign in
void showLoginMenu() {
	system("cls");
	int choise;
	std::cout << "Welcome to Task Manager" << std::endl;
			std::cout << "1. Log in\n2. Sign in" << std::endl;
			choise = getValidatedInput<int>("Choose an option : ");
			switch(choise) {
				case 1:
					logIn();
					break;
				case 2:
					signIn();
					break;
				default:
					showLoginMenu();
			}
}



// function untuk memberikan ucapan kepada user sesuai waktu real time
void sayHello(std::string time) {
	std::cout << "Good " << time <<" "<<  curentUser.name << std::endl;
}

// function untuk mendapatkan waktu real time 
std::string getTimeOfDay() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);
    int hour = localTime->tm_hour;
    if (hour >= 5 && hour < 12) {
        return "Morning";
    } else if (hour >= 12 && hour < 15) {
        return "Afternoon";
    } else if (hour >= 15 && hour < 18) {
        return "Afternoon";
    } else {
        return "Evening";
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
	// std::cin.ignore();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    std::string date = getSingleWordInput("Masukkan tanggal (DD-MM-YYYY): ");
		if (date.length() == 0) {
			date = getCurrentDate();
		}
    std::string time =  getSingleWordInput("Masukkan jam (HH:MM): ");
		if (time.length() == 0) {
			time = "23:59";
		}
    std::string datetime = convertToMySQLDatetime(date, time);
    std::cout << datetime ;
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

// function untuk menampilkan data task dalam bentuk tabel
bool displayAllTasksData() {
	if (!taskData.size()) {
		std::cout <<"Your task data is empty !" << std::endl;
		return false;
	}
	int numberTask;
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

	return true;
}

// function untuk memuat semua data task dari database ke dalam program
void seeAllTaskData() {
	system("cls");
	getDataTaskByUsername(curentUser.username, taskData);
	sortTasks(taskData);
	int numberTask;
	if (displayAllTasksData()) {
			// untuk melihat task lebih detail
		std::cout << "See task number = ";
		std::cin >> numberTask;
		if (numberTask == 0) {
			return;
		}
		if (numberTask < 1 || numberTask > taskData.size()) {
			seeAllTaskData();
		}
		displayTaskDetail(numberTask);	
		menuOnTaskDetail(numberTask);
		seeAllTaskData();
	}
}

// funtion untuk menampilkan menu ke user
void menu() {
	getDataTaskByUsername(curentUser.username, taskData);
	std::string searchWord;
	int choise;
		system("cls");
		sayHello(getTimeOfDay());
		menuOptios(choise);
		switch (choise) {
		case  1: 
			system("cls");
			// menu menampilkan semua task
			seeAllTaskData();
			break;
		case  2: 
			system("cls");
			// menu menambah task
			addTask(curentUser.id);
			break;
		case  3: 
			// menu mencari task 
			system("cls");
			searchWord = getInputWithSpaces("Please enter a keyword to search for the task name : ");
			searchTasks(searchWord, taskData);
			break;
		case 4: 
			// menu menghapus task 
			system("cls");
			deleteTasks();
			break;
		case  5: 
			// menu logout dari program
			system("cls");
			logout();
			return;
			break;
		default :
			break;
		}
		system("pause");
		menu();
}

void menuOptios(int &choise) {
	std::cout << "1. Display All Taks" << std::endl;
	std::cout << "2. Add New Task" << std::endl;
	std::cout << "3. Search Task" << std::endl;
	std::cout << "4. Delete Task" << std::endl;
	std::cout << "5. Logout" << std::endl;
	// std::cout << "Choose an option : ";
	choise = getValidatedInput<int>("Choose an option : ");
}

// function untuk sistem login
void logIn() {
	system("cls");
	std::string username;
	std::string password;
	std::cout << "======= Log In =======" << std::endl;
	username = getSingleWordInput("Username     : ");
	password = getSingleWordInput("Password     : ");
	if(checkLogin(username, password)) {
		std::cout << "Login Successful....." << std::endl;
		Sleep(2000);
		getCurrentDataUser(username, curentUser, userData);
		createCache(username, password);
		menu();
	} else {
		std::cout << "Login Failed....." << std::endl;
		Sleep(2000);
		logIn();
	}
}

// funtion untuk sistem signin
void signIn() {
	system("cls");
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
	name = getInputWithSpaces("Name         : ");
	std::cout << "\n         ======= USERNAME =======" << std::endl;
	std::cout << "+-------------------------------------------+" << std::endl;
	std::cout << "|    Username must have 4 - 12 characters   |" << std::endl;
	std::cout << "|    Username must start with a letter      |" << std::endl;
	std::cout << "+-------------------------------------------+" << std::endl;
	username = getSingleWordInput("Username     : ");
	std::cout << "\n         ======= PASSWORD =======" << std::endl;
	std::cout << "+-------------------------------------------+" << std::endl;
	std::cout << "|  Password must contain 8 - 16 characters  |" << std::endl;
	std::cout << "|  Password must consist of a combination   |" << std::endl;
	std::cout << "|           of leters and numbers           |" << std::endl;
	std::cout << "+-------------------------------------------+" << std::endl;
	password1 = getSingleWordInput("Password     : ");
	password2 = getSingleWordInput("Confirmation : ");

	if (validateData(name, username, password1, password2)) {
		if(addDataUser(capitalizeFIrstLetter(name), username, password1)) {
			getUserData();
			createCache(username, password1);
			getCurrentDataUser(username, curentUser, userData);
			menu();
			return;
		} 
	}

	signIn();
}

// function untuk log out / keluar dari program
void logout() {
	deleteCache();
}
