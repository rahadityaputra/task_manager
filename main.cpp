#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <string.h>
#include <vector>
#include <cstring>
#include <fstream>

#define MAX_USERNAME_LENGTH 12
#define MIN_USERNAME_LENGTH 4
#define MAX_PASSWORD_LENGTH 16
#define MIN_PASSWORD_LENGTH 8
#define MAX_NAME_LENGTH 100
#define MIN_NAME_LENGTH 1


// TODO

// MEMBUAT FUNCTION UNTUK MEMVALIDASI USERNAME DAN PASSWORD SAAT USER DAFTAR
// MEMBUAT MENU UTAMA
// MERAPIKAN ERD DATABASE YANG TELAH DIBUAT
// MENGHAPUS SEMUA DATA YANG ADA DI DATABASE
// MEMBUAT FUNCTION SAY HELLO
// MERAPIKAN FORM LOGIN DENGAN LIBRARY IOMANIP



// INi Editan
struct USER {
	std::string id;
	std::string username;
	std::string password;
	std::string created_at;
};

std::ofstream cacheWrite;
std::ifstream cacheRead;

MYSQL* conn;
MYSQL_RES* result;
MYSQL_ROW row;

std::vector<USER> userData;

char host[] = "localhost";
char username[] = "root";
char password[] = "";
char database[] = "task_manager_db";
int port = 3306;

void sayHello(std::string name) {
	std::cout << "Hello " << name << std::endl;
}

bool validateData(std::string name, std::string username, std::string password1, std::string password2) {
	int nameLength = name.length();
	int usernameLength = username.length();
	int password1Length = password1.length();
	int password2Length = password2.length();
	
	if (nameLength >= MIN_NAME_LENGTH && nameLength <= MAX_NAME_LENGTH) {
		if (usernameLength >= MIN_USERNAME_LENGTH && usernameLength <= Max_USERNAME_LENGTH) {
				if (password1Length >= MIN_PASSWORD_LENGTH && password1Length <= MAX_PASSWORD_LENGTH && password2Length >= MIN_PASSWORD_LENGTH && password2Length <= MAX_PASSWORD_LENGTH  ) {
					if (password1 == password2) {
						return true;
					} else {
						std::cout << "password tidak sama" << std::endl;
					}
				} else {
					std::cout << "password harus 8 - 16 karakter" << std::endl;
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



// function untuk mendapatkan koneksi dari mysql
void getMysqlConnection() {
	if((conn = mysql_init(NULL))) {
		conn = mysql_real_connect(conn, host, username, password, database, port, NULL, 0);	
	}
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

char* getUsernameDataInCacheFile() {
	char* username;
	cacheRead.open("cache.txt");
	cacheRead >> username;
	cacheRead >> username;
	cacheRead.close();
	return username;
}


std::string getNameDataByUsername(char username[50]) {
	char query[500];
	char escaped_username[100];
	mysql_real_escape_string(conn, escaped_username, username, strlen(username));
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

void menu() {
	system("cls");
	std::cout << "ini menu";
	int choise;
	std::cout << "selamat datang di menu utama" << std::endl;
	std::cout << "0. log out" << std::endl;
	std::cout << "choise : ";
	std::cin >> choise;
	if(choise == 0) {
		deleteCache();
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
		menu();
	} else {
		logIn();
		
	}
}

void signIn() {
	system("cls");
	std::string name;
	std::string username;
	std::string password1;
	std::string password2;
	std::cout << "======= Sign In =======" << std::endl;
	std::cout << "Name : ";
	std::cin >> name;
	std::cout << "Username : ";
	std::cin >> username;
	std::cout << "Password : ";
	std::cin >> password1;
	std::cout << "Password : ";
	std::cin >> password2;

	if (validateData(name, username, password1, password2)) {
		if(addDataUser(name, username, password1)) {
			createCache(username, password1);
			menu();	
		}
	} else {
		std::cout << "pw 1 dan pw 2 beda" << std::endl;
	}
}


int main() {
	getMysqlConnection();
	
	int choise;
	if (conn) {
		if(checkCache()) {
			menu();
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
