#include <iostream>
#include <sstream>

using namespace std;

int main() {
	int startingData = 6;
	int addressIntForm = 5;
	string address = "0";
	int* testInteger = &startingData;
	string input = "a";

	while (input != "q") {
		cout << "Enter 'a' to view the address held by the pointer, Enter 'c' to change the address held by the pointer, Enter 'i' to view the value at the address held by the pointer" << endl;
		cin >> input;
		if (input == "a") {
			cout << "address held by the pointer: " << testInteger << endl << endl;
		}
		if (input == "c") {
			cout << "change the address held by the integer pointer: " << endl;
			cin >> address;
			std::stringstream ss;
			ss << std::hex << address;
			ss >> addressIntForm;
			testInteger = (int*) addressIntForm;
			cout << endl << endl;
		}
		else if (input == "i") {
			cout << "value at the address pointed to by the pointer is: " << *testInteger << endl << endl;
		}
	}
}