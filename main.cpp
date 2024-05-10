#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <string>


MYSQL* conn;
MYSQL_RES* result;
MYSQL_ROW row;


char host[] = "localhost";
char username[] = "root";
char password[] = "";
char database[] = "task_manager_db";
int port = 3306;

// struct untuk menampung data user dari mydql
struct user {
	int id;
	char username[50];
	char password[225];
	char created_at[50];
};


// function untuk mendapatkan koneksi dari mysql
bool getMysqlConnection(MYSQL* conn) {
	if(!(conn = mysql_init(NULL))) {
	//	std::cout << "init gagal "<< std::endl;
	} else {
	//	std::cout << "init berhasil "<< std::endl;
	//	std::cout << conn << std::endl;
		// membuat koneksi ke mysql
		conn = mysql_real_connect(conn, host, username, password, database, port, NULL, 0);
		// cek apakah berhasil koneksi ke database mysql;
		if(conn) {
	//		std::cout << "koneksi berhasil" << std::endl;	
			// jika koneksi berhasil
			return true;								
		} else {
	//		std::cout << "koenksi gagal" << std::endl;
		}								
	}
	
	// jika koneksi gagal
	return false;
}

bool logIn() {
	char username[50];
	char password[225];
	std::cout << "======= Log In =======" << std::endl;
	std::cout << "Username : ";
	std::cin >> username;
	std::cout << "Password : ";
	std::cin >> password; 
	std::cout << username << " "<< password ;
	
	//cek jika user memasukkan data yang benar , maka user dilempar ke menu utama
	//cek jika user salah, maka user dilempar ke menu sign in
	
}

bool signIn() {
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
	
	// cek apakah password1 dan password2 sama , jika sama maka 
		// cek apakah username sudah ada sebelumnya jika tidak ada maka ulang function ini
	// jika password1 dan password2 berbeda ulang funtion ini
	if (password1 == password2) {
		std::cout << "cek username di database";
	} else {
		
	}
	
	std::cout << username << " "<< password ;
}






int main() {
	
	// ini dibuthkan saat user selesai input data username dan password
	bool mysqlConn = getMysqlConnection(conn);
	
	int choise;
	
	if (mysqlConn) {
		std::cout << "Welcome to Task Manager" << std::endl;
		std::cout << "1. Log in\n2. Sign in" << std::endl;
		std::cout << "Choise = " ; 
		std::cin >> choise;
		
		switch(choise) {
			case 1:
				logIn();
				break;
			case 2:
				std::cout << "sing in menu" ;
				signIn();
				break;		
			default:
				break;
		}
	} else {
		std::cout << "koneksi error" << std::endl;
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
