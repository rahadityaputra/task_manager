#include <mysql.h>
#include <mysqld_error.h>
#include "user.h"
#include "task.h"

MYSQL* conn;
MYSQL_RES* result;
MYSQL_ROW row;

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

// funtion untuk mendapatkan semua data user yang ada di database
void getUserData() {
	const char *query = "SELECT * FROM user";
	if(!(mysql_query(conn,  query))) {
		result = mysql_use_result(conn);
		while (row = mysql_fetch_row(result)) {
			std::string column0 = row[0];
      std::string column1 = row[1];
      std::string column2 = row[2];
      std::string column3 = row[3];
      std::string column4 = row[4];
			userData.push_back({column0, column1, column2, column3, column4});
		}
	} else {
		std::cout << mysql_error(conn) ;
	}	
	mysql_free_result(result);
}

// function untuk mendapatkan semua data task yang dimiliki seorang user
void getDataTaskByUsername(std::string username) {
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
        std::cout << "Gagal menambahkan data! Error: " << mysql_error(conn) << std::endl;
        return false;
    }
}
