#include <iostream>
#include <string>

using namespace std;

std::string get_password() {
	string password;
	cout << "Enter password(16 characters max):" << endl;
	std::getline(cin, password);

	if (password.empty() || password.length() > 16) {
		cerr << "Password is too long. Please enter a password with a maximum of 16 characters." << endl;
		return get_password();
	}
    if (password.length() < 16) {
        password.append(16 - password.length(), '0');
    }
	return password;
}