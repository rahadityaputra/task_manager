#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <string.h>
#include <vector>
#include <cstring>

// struct untuk menampung data user dari mydql
//struct USER {
//	char id[255];
//	char username[50];
//	char password[225];
//	char created_at[50];
//};

// INi Editan
struct USER {
	std::string id;
	std::string username;
	std::string password;
	std::string created_at;
};

MYSQL* conn;
MYSQL_RES* result;
MYSQL_ROW row;

std::vector<USER> userData;

char host[] = "localhost";
char username[] = "root";
char password[] = "";
char database[] = "task_manager_db";
int port = 3306;




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
			//std::cout << row[0] << " " << row[1] << " " << row[2] << " " << row[3] << std::endl;
			userData.push_back({column0, column1, column2, column3});
		}
			int count = mysql_num_rows(result);
		std::cout << "jumlah data = " << count;
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

void addDataUser(const char* username, const char* password) {
    char query[500];
    char escaped_username[100], escaped_password[450];
    mysql_real_escape_string(conn, escaped_username, username, strlen(username));
    mysql_real_escape_string(conn, escaped_password, password, strlen(password));
    sprintf(query, "INSERT INTO user VALUES (NULL, '%s', '%s', current_timestamp())", escaped_username, escaped_password);
    if (mysql_query(conn, query) == 0) {
        std::cout << "Data berhasil ditambahkan!" << std::endl;
    } else {
        std::cerr << "Gagal menambahkan data! Error: " << mysql_error(conn) << std::endl;
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

void logIn() {
	char username[50];
	char password[225];
	std::cout << "======= Log In =======" << std::endl;
	std::cout << "Username : ";
	std::cin >> username;
	std::cout << "Password : ";
	std::cin >> password; 
	getUserData();
	if(checkLogin(username, password)) {
		std::cout << "masuk menu" << std::endl;
	} else {
		logIn();
	}
	
	
}

void signIn() {
	char username[50];
	char password1[225];
	char password2[225];
	std::cout << "======= Sign In =======" << std::endl;
	std::cout << "Username : ";
	std::cin >> username;
	std::cout << "Password : ";
	std::cin >> password1;
	std::cout << "Password : ";
	std::cin >> password2;

	if (strcmp(password1, password2) == 0) {
		addDataUser(username, password1);
		
	} else {
		std::cout << "pw 1 dan pw 2 beda" << std::endl;
	}
}

int main() {
	
	getMysqlConnection();
	int choise;
	if (conn) {
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
	} else {
		std::cout << "Koneksi Database Eror" << std::endl;
	}
	
	
	
	
	
	
	// membuat form login
	// membuat form sign in
		//-saat sign in user diminta untuk memasukkan password 2 kali
	
	//std::cout << koneksi ;
	
//		if((!mysql_query(conn, "SELECT * FROM students"))) { 
//						result = mysql_use_result(conn);
//						while(row = mysql_fetch_row(result)) {
//							std::cout <<  "ID = "<< row[0] << std::endl;
//							std::cout <<  "Email = "<< row[1] << std::endl;
//							std::cout <<  "Password = "<< row[2] << std::endl;
//							std::cout <<  "Name = "<< row[3] << std::endl;
//							std::cout <<  "NIM = "<< row[4] << std::endl;
//							std::cout << std::endl;
//						}
//				}
	
	return 0;
}
