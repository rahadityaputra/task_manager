#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>


MYSQL* conn;
MYSQL_RES* result;
MYSQL_ROW row;


char host[] = "localhost";
char username[] = "root";
char password[] = "";
char database[] = "students_database";
int port = 3306;

int main() {
	std::cout << "Selamat datang di mysql connection" << std::endl;
	
	conn = mysql_init(NULL);
	if(conn) {
		conn = mysql_real_connect(conn, host, username, password, database, port, NULL, 0);
		// cek apakah berhasil koneksi ke database mysql;
		if (conn) {
			std::cout << "koneksi berhasil" << std::endl;
			
			if((!mysql_query(conn, "SELECT * FROM students"))) { 
				result = mysql_use_result(conn);
				while(row = mysql_fetch_row(result)) {
					std::cout <<  "ID = "<< row[0] << std::endl;
					std::cout <<  "Email = "<< row[1] << std::endl;
					std::cout <<  "Password = "<< row[2] << std::endl;
					std::cout <<  "Name = "<< row[3] << std::endl;
					std::cout <<  "NIM = "<< row[4] << std::endl;
					std::cout << std::endl;
				}
				
				
				mysql_free_result(result);
				mysql_close(conn);
				
			} else {
				std::cout << "query salah !" ;
			}
			
			
		} else {
			std::cout << "koenksi gagal" << std::endl;
		}
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}
	
	return 0;
}
