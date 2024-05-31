
#include <iostream>
#include "user.h"
#include <windows.h>
#include <sstream>

#define MAX_USERNAME_LENGTH 25
#define MIN_USERNAME_LENGTH 6
#define MAX_PASSWORD_LENGTH 16
#define MIN_PASSWORD_LENGTH 8
#define MAX_NAME_LENGTH 100
#define MIN_NAME_LENGTH 1

// function untuk cek apakah username dan password yang diinputkan saat login ada atau tidak
bool  checkLogin(std::string username, std::string password) {
	for (int i = 0; i < userData.size(); i++) {
		if (username == userData[i].username && password == userData[i].password) {
			return true;
		}	
  }
  return false;	
}

// funtion untuk cek valid atau tidak name yang dimasukkan oleh user
bool isValidName(const std::string& name) {
    if (name.empty()) {
			std::cout << "Name must be filled in !" << std::endl;
        return false;
    }
    for (int i = 0; i < name.length(); i++) {
        if (std::isdigit(name[i])) {
            std::cout << "Name must not contain numbers !" << std::endl ;
            return false;
        }
    }
    return true;
}

// function untuk cek valid atau tidak username yang dimasukkan oleh user
bool isValidUsername(const std::string& username) {
    if (username.empty()) {
			std::cout << "Username must be filled in !" << std::endl;
        return false;
    }
    if( std::isalpha(username[0])) {
			return true;
		} else {
			std::cout << "Username must begin with a letter !" << std::endl; 
		}
}

// function untuk cek valid atau tidak password yang dimasukkan oleh user
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
    std::cout << "Password must be a combination of letters and numbers." << std::endl;
    return hasLetter && hasDigit;
}

// funtion untuk memvalidasi data sign in user
bool validateData(std::string name, std::string username, std::string password1, std::string password2) {
	int nameLength = name.length();
	int usernameLength = username.length();
	int password1Length = password1.length();
	int password2Length = password2.length();
	if (isValidName(name)) {
        if (nameLength >= MIN_NAME_LENGTH && nameLength <= MAX_NAME_LENGTH) {
            if (usernameLength >= MIN_USERNAME_LENGTH && usernameLength <= MAX_USERNAME_LENGTH) {
                if (isValidUsername(username)) {
                    if (password1Length >= MIN_PASSWORD_LENGTH && password1Length <= MAX_PASSWORD_LENGTH && password2Length >= MIN_PASSWORD_LENGTH && password2Length <= MAX_PASSWORD_LENGTH  ) {
                        if (password1 == password2) {
                            if(isValidPassword(password1)) {
                                return true;
                            }
                        }
                    }	
                }
            } 
        }       
    }
	return false;
}

// funtion untuk mendapatkan input yang sesuai yang diminta oleh program

template<typename T>
T getValidatedInput(std::string prompt) {
 T input;
    while (true) {
      std::cout << prompt;
      std::cin >> input;
        if (std::cin.fail()) {
            // Kesalahan input: tipe data tidak sesuai
            std::cin.clear(); // Menghapus flag kesalahan
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Mengabaikan input yang salah
          std::cout << "Invalid input. Please enter a valid value." <<std::endl;
        } else {
            // Input valid
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Mengabaikan sisa input yang tidak diinginkan
            break;
        }
    }
    return input;
}

// Fungsi untuk mendapatkan input satu kata yang tervalidasi
std::string getSingleWordInput(const std::string& prompt) {
    std::cin.ignore();
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        std::istringstream stream(input);
        std::string word;
        stream >> word;

        // Periksa apakah ada lebih dari satu kata
        if (stream >> input) {
            std::cout << "Invalid input. Please enter only one word." << std::endl;
        } else {
            return word;
        }
    }
}


