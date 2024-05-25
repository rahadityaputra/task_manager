#include <iostream>

#define MAX_USERNAME_LENGTH 25
#define MIN_USERNAME_LENGTH 6
#define MAX_PASSWORD_LENGTH 16
#define MIN_PASSWORD_LENGTH 8
#define MAX_NAME_LENGTH 100
#define MIN_NAME_LENGTH 1

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
