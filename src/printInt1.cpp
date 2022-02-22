#include <iostream>
#include <sstream>

using namespace std;

int main() {
	int testInt;
	char input = 'a';
	cout << "Input an integer: " << endl;
	cin >> testInt;
	cout << endl;

	while (input != 'q') {
		cout << "Enter 'i' to view your int, Enter 'a' to view the address of your int" << endl;
		cin >> input;
		if (input == 'i') {
			cout << testInt << endl << endl;
		}
		else if (input == 'a') {
			cout << &testInt << endl << endl;
		}
	}
}