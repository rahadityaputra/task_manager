#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <mysqld_error.h>
#include "user.h"
#include "task.h"
#include <sstream>
#include <algorithm>
#include <iomanip>

// Deklarasi variabel conn menggunakan extern
extern MYSQL *conn;
extern MYSQL_RES* result;
extern MYSQL_ROW row;

extern char host[];
extern char username[];
extern char password[];
extern char database[];
extern int port;

// Fungsi untuk menginisialisasi koneksi
void getMysqlConnection();
void getUserData(); 
void getDataTaskByUsername(std::string username, std::vector<TASK>& taskData);
bool addTaskData(std::string task_name, std::string description, std::string deadline, std::string  user_id);
bool addDataUser(std::string name, std::string username, std::string password);
void displayTaskDetail(int number );
void searchTasks(std::string searchWord, std::vector<TASK>& taskData);
bool deleteTasksData(std::string id_task);
tm stringToTime(const std::string& datetime);
void sortTasks(std::vector<TASK>& taskData);
#endif // DATABASE_H
