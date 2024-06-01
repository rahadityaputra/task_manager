#include "database.h"



// Definisi variabel
MYSQL *conn = nullptr;
MYSQL_RES* result = nullptr;
MYSQL_ROW row = nullptr;

char host[] = "localhost";
char username[] = "root";
char password[] = "";
char database[] = "task_manager_db";
int port = 3306;

// funtion untuk delete task
bool deleteTasksData(std::string id_task) {
	// const char *query = "DELETE FROM task WHERE `task`.`task_id` = 23";
	char query[500];
	char escaped_id_task[200];
	mysql_real_escape_string(conn, escaped_id_task, id_task.c_str(), id_task.length());
	sprintf(query,  "DELETE FROM task WHERE `task`.`task_id` = '%s'", escaped_id_task);
	if(!(mysql_query(conn,  query))) {
		std::cout << "The task has been successfully deleted" << std::endl;
		return true;
	} else {
		std::cout << mysql_error(conn) ;
		return false;
	}
	mysql_free_result(result);
}

// function untuk mendapatkan koneksi dari mysql
void getMysqlConnection() {
	if((conn = mysql_init(NULL))) {
		conn = mysql_real_connect(conn, host, username, password, database, port, NULL, 0);	
		std::cout << mysql_error(conn) << std::endl;
	}
}

// funtion untuk mendapatkan semua data user yang ada di database
void getUserData() {
	const char *query = "SELECT * FROM user";
	if(!(mysql_query(conn,  query))) {
		userData.clear();
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
void getDataTaskByUsername(std::string username, std::vector<TASK>& taskData) {
	// data task data akan dihapus semua dan akan diganti dengan data yang baru
	taskData.clear();
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
        std::cout << "Task has been successfullt added !" << std::endl;
        return true;
    } else {
        std::cerr << "Failed to add task ! Error :" << mysql_error(conn) << std::endl;
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
        std::cout << "Sign In Successfully ...." << std::endl;
				Sleep(2000);
        return true;
    } else {
        // std::cout << "Gagal menambahkan data! Error: " << mysql_error(conn) << std::endl;
				std::cout << "The username you have entered is already in use by another user. Please try a different username.." << std::endl;
				Sleep(2000);
        return false;
    }
}

// funtion untuk melihat detail task
void displayTaskDetail(int number ) {
	int index = number - 1;
	int width = std::max({
			taskData[index].task_name.size(),
			taskData[index].description.size(),
			taskData[index].deadline.size(),
			taskData[index].created_at.size()
	}) + 2;

	std::string border(width + 20, '-'); // Tambahkan panjang label
	std::cout << "+" << border << "+" << std::endl;
	std::cout << "| Task Detail" << std::setw(width + 7) << " |" << std::endl; // +7 untuk mengimbangi tambahan di label
	std::cout << "+" << border << "+" << std::endl;
	std::cout << "| Title       : " << std::setw(width) << std::left << taskData[index].task_name << " |" << std::endl;
	std::cout << "+" << border << "+" << std::endl;
	std::cout << "| Description : " << std::setw(width) << std::left << taskData[index].description << " |" << std::endl;
	std::cout << "+" << border << "+" << std::endl;
	std::cout << "| Deadline    : " << std::setw(width) << std::left << taskData[index].deadline << " |" << std::endl;
	std::cout << "+" << border << "+" << std::endl;
	std::cout << "| Created at  : " << std::setw(width) << std::left << taskData[index].created_at << " |" << std::endl;
	std::cout << "+" << border << "+" << std::endl;
}

// funtion untuk mencari task dengan title judul 
void searchTasks(std::string searchWord, std::vector<TASK>& taskData) {
	
	for (int i = 0; i < taskData.size(); i++) {
		if (taskData[i].task_name.find(searchWord) != std::string::npos) {
			displayTaskDetail(i + 1);
		}
	}
}

// Fungsi untuk mengubah string tanggal ke format struct tm
tm stringToTime(const std::string& datetime) {
    std::tm tm = {};
    sscanf(datetime.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", 
           &tm.tm_year, &tm.tm_mon, &tm.tm_mday, 
           &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    tm.tm_year -= 1900; // Tahun dihitung sejak 1900
    tm.tm_mon -= 1;     // Bulan dari 0-11
    return tm;

}


// Fungsi Bubble Sort untuk mengurutkan array tanggal
void sortTasks(std::vector<TASK>& taskData) {
	int n = taskData.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            std::tm tm1 = stringToTime(taskData[j].deadline);
            std::tm tm2 = stringToTime(taskData[j + 1].deadline);
            if (mktime(&tm1) > mktime(&tm2)) {
                std::swap(taskData[j], taskData[j + 1]);
            }
        }
    }
}
