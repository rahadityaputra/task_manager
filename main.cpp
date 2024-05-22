#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <string.h>
#include <vector>
#include <cstring>
#include <fstream>
#include <cctype>
#include <chrono>
#include <ctime>
#include "username.h"
#include <iomanip>
#include <sstream>


// MEMBUAT MENU UTAMA
// MERAPIKAN ERD DATABASE YANG TELAH DIBUAT
// MERAPIKAN FORM LOGIN DENGAN LIBRARY IOMANIP
// membuat funtion untuk menambahkan data task

struct USER {
	std::string id;
	std::string username;
	std::string password;
	std::string created_at;
};

struct TASK {
	std::string id;
	std::string task_name;
	std::string description;
	std::string deadline;
	std::string created_at;
	std::string user_id;
};

std::ofstream cacheWrite;
std::ifstream cacheRead;

MYSQL* conn;
MYSQL_RES* result;
MYSQL_ROW row;

std::vector<USER> userData;
std::vector<TASK> taskData;


char host[] = "localhost";
char username[] = "root";
char password[] = "";
char database[] = "task_manager_db";
int port = 3306;

void sayHello(std::string name, std::string time) {
	std::cout << "Selamat " << time <<" "<<  name << std::endl;
}

bool isValidUsername(const std::string& username) {
    if (username.empty()) {
			std::cout << "Username harus diisi !" << std::endl;
        return false;
    }
    if( std::isalpha(username[0])) {
			return true;
		} else {
			std::cout << "Username harus diawali huruf !" << std::endl; 
		}
}

bool isValidPassword(const std::string& password) {
    bool hasLetter = false;
    bool hasDigit = false;

    for (char ch : password) {
        if (std::isalpha(ch)) {
            hasLetter = true;
        }
        if (std::isdigit(ch)) {
            hasDigit = true;
        }
        if (hasLetter && hasDigit) {
            return true;
        } 
    }

		std::cout << "Password harus kombinasi karakter huruf dan angka" << std::endl;
    return hasLetter && hasDigit;
}

std::string getTimeOfDay() {
    // Ambil waktu sekarang
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

bool addTaskData(std::string task_name, std::string description, std::string deadline, std::string  user_id) {
	char query[500];
	char escaped_task_name[100], escaped_descriptio[100], escaped_deadlinee[100], escaped_user_id[100];
	mysql_real_escape_string(conn, escaped_task_name, task_name.c_str(), task_name.length());
	mysql_real_escape_string(conn, escaped_descriptio, description.c_str(), description.length());
	mysql_real_escape_string(conn, escaped_deadlinee, deadline.c_str(), deadline.length());
	mysql_real_escape_string(conn, escaped_user_id, user_id.c_str(), user_id.length());
	sprintf(query, "INSERT INTO task VALUES (NULL, '%s', '%s', '%s', current_timestamp(), '%s')", escaped_task_name, escaped_descriptio, escaped_deadlinee, escaped_user_id);
	if (mysql_query(conn, query) == 0) {
        std::cout << "Data berhasil ditambahkan!" << std::endl;
        return true;
    } else {
        std::cerr << "Gagal menambahkan data! Error: " << mysql_error(conn) << std::endl;
        return false;
    }
}



std::string ubah_huruf_pertama_kapital(std::string nama) {
	int jumlah_huruf = nama.length();
	bool setelah_spasi = true;
	for (int i = 0; i < jumlah_huruf; i++) {
		if (nama[i] == ' ') {
			setelah_spasi = true;
			continue;
		} 
		if (setelah_spasi) {
			if (nama[i] >= 'a' && nama[i] <= 'z' ) {
      	nama[i] = nama[i] - 32;
			} 
        setelah_spasi = false;
   	 } 
		else if (!setelah_spasi) {
		 if (nama[i] >= 'A' && nama[i] <= 'Z') {
        nama[i] = nama[i] + 32;
        setelah_spasi = false;
			}			
		}
	}
	return nama;
}

bool validateData(std::string name, std::string username, std::string password1, std::string password2) {
	int nameLength = name.length();
	int usernameLength = username.length();
	int password1Length = password1.length();
	int password2Length = password2.length();
	
	if (nameLength >= MIN_NAME_LENGTH && nameLength <= MAX_NAME_LENGTH) {
		if (usernameLength >= MIN_USERNAME_LENGTH && usernameLength <= MAX_USERNAME_LENGTH) {
			if (isValidUsername(username)) {
				if (password1Length >= MIN_PASSWORD_LENGTH && password1Length <= MAX_PASSWORD_LENGTH && password2Length >= MIN_PASSWORD_LENGTH && password2Length <= MAX_PASSWORD_LENGTH  ) {
					if (password1 == password2) {
						if(isValidPassword(password1)) {
							return true;
						}
					} else {
						std::cout << "password tidak sama" << std::endl;
					}
				} else {
					std::cout << "password harus 8 - 16 karakter" << std::endl;
				}		
			}
		} else {
			std::cout << "Username harus 4 sampai 12 karakter" << std::endl;
		}
	
	} else {
		std::cout << "nama harus 1 - 100 karakter" <<std:: endl;
	}

	return false;
}


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

void createCache(std::string username, std::string password) {
	cacheWrite.open("cache.txt");
	cacheWrite << "true";
	cacheWrite << "\n" << username;
	cacheWrite << "\n" << password;
	cacheWrite.close();
}

void deleteCache() {
	cacheWrite.open("cache.txt");
	cacheWrite << "false";
	cacheWrite.close();
}

std::string getInputWithSpaces(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

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


void addTask(std::string user_id) {
    std::string task_name = getInputWithSpaces("Masukkan task name: ");
    std::string description = getInputWithSpaces("Masukkan description: ");
    std::string date = getInputWithSpaces("Masukkan tanggal (DD-MM-YYYY): ");
    std::string time = getInputWithSpaces("Masukkan jam (HH:MM): ");
    std::string datetime = convertToMySQLDatetime(date, time);

    addTaskData(task_name, description, datetime, user_id);
    std::cout << "Task berhasil ditambahkan!" << std::endl;
}



// function untuk mendapatkan koneksi dari mysql
void getMysqlConnection() {
	if((conn = mysql_init(NULL))) {
		conn = mysql_real_connect(conn, host, username, password, database, port, NULL, 0);	
	}
}

void getDataTaskByUsername(std::string username) {
	// SELECT * FROM task WHERE user_id = (SELECT id FROM user WHERE username = 'abid123');
	char query[500];
	char escaped_username[100];
	mysql_real_escape_string(conn, escaped_username, username.c_str(), username.length());
	sprintf(query,  "SELECT * FROM task WHERE user_id = (SELECT id FROM user WHERE username = '%s')", escaped_username);
	if(!(mysql_query(conn,  query))) {
		result = mysql_use_result(conn);
		while (row = mysql_fetch_row(result)) {
			std::string column0 = row[0];
			std::string column1 = row[1];
			std::string column2 = row[2];
			std::string column3 = row[3];
			std::string column4 = row[4];
			std::string column5 = row[5];
			taskData.push_back({column0, column1, column2, column3, column4, column5});
		}
	} else {
		std::cout << mysql_error(conn) ;
	}
	mysql_free_result(result);

}


void getUserData() {
	const char *query = "SELECT * FROM user";
	if(!(mysql_query(conn,  query))) {
		result = mysql_use_result(conn);
		while (row = mysql_fetch_row(result)) {
			std::string column0 = row[0];
        	std::string column1 = row[1];
        	std::string column2 = row[2];
        	std::string column3 = row[3];
			userData.push_back({column0, column1, column2, column3});
		}
	} else {
		std::cout << mysql_error(conn) ;
	}
	
	mysql_free_result(result);
}

std::string getUsernameDataInCacheFile() {
	std::string username;
	cacheRead.open("cache.txt");
	cacheRead >> username;
	cacheRead >> username;
	cacheRead.close();
	return username;
}


std::string getNameDataByUsername(std::string username) {
	char query[500];
	char escaped_username[100];
	mysql_real_escape_string(conn, escaped_username, username.c_str(), username.length());
	sprintf(query,  "SELECT * FROM user WHERE username = '%s'", escaped_username);
	if(!(mysql_query(conn,  query))) {
		std::string name;
		result = mysql_use_result(conn);
		while (row = mysql_fetch_row(result)) {
        	 name = row[3];
		}	
	return name;
	} else {
		std::cout << mysql_error(conn) ;
	}
	mysql_free_result(result);
}



void displayData() {
	for (int i = 0; i < userData.size(); i++) {
    std::cout << "id " << userData[i].id  << std::endl;
    std::cout << "username : " << userData[i].username << std::endl;
    std::cout << "password : " << userData[i].password << std::endl;
    std::cout << "create : "<< userData[i].created_at << std::endl;;
  }
}


bool addDataUser(std::string name, std::string username, std::string password) {
    char query[500];
    char escaped_username[100], escaped_password[100], escaped_name[100];
    mysql_real_escape_string(conn, escaped_username, username.c_str(), username.length());
    mysql_real_escape_string(conn, escaped_password, password.c_str(), password.length());
	mysql_real_escape_string(conn, escaped_name, name.c_str(), name.length());
    sprintf(query, "INSERT INTO user VALUES (NULL, '%s', '%s', '%s', current_timestamp())", escaped_username, escaped_password, escaped_name);
    if (mysql_query(conn, query) == 0) {
        std::cout << "Data berhasil ditambahkan!" << std::endl;
        return true;
    } else {
        std::cerr << "Gagal menambahkan data! Error: " << mysql_error(conn) << std::endl;
        return false;
    }
}


bool  checkLogin(std::string username, std::string password) {
	for (int i = 0; i < userData.size(); i++) {
		if (username == userData[i].username && password == userData[i].password) {
			return true;
		}	
  }
  
  return false;
}


// void loadTaskData() {
// 	std::cout << "TASK LIST" << std::endl;
// 	for (int i = 0; i < taskData.size(); i++) {
// 		std::cout << i + 1<<". "<< taskData[i].task_name << std::endl;
// 	}
// }
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

void menu(std::string username) {
	system("cls");
	sayHello(getNameDataByUsername(username), getTimeOfDay());
	int choise;
	getDataTaskByUsername(username);
	loadTaskData();

	std::cout << "2. Add Task" << std::endl;
	std::cout << "0. log out" << std::endl;
	std::cout << "choise : ";
	std::cin >> choise;
	// if(choise == 0) {
	// 	deleteCache();
	// }
	switch (choise) {
	case  0: 
		deleteCache();
	break;
	case  2: 
		addTask("19");
	break;
	default :
		
	}
}

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
		std::cout << "masuk menu" << std::endl;
		createCache(username, password);
		menu(username);
	} else {
		logIn();
		
	}
}

void signIn() {
	// system("cls");
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
		if(addDataUser(ubah_huruf_pertama_kapital(name), username, password1)) {
			createCache(username, password1);
			menu(username);
		}
	} else {
		signIn();
	}
}

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
