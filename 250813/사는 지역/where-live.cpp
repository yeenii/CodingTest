#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>

using namespace std;

const int n_max = 10;
int n;

class human {
public:
	string name;
	string addr;
	string location;

	human(string name="", string addr="", string location="") {
		this->name = name;
		this->addr = addr;
		this->location = location;
	};
};

human hu[n_max];

int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		string name;
		string addr;
		string location;

		cin >> name >> addr >> location;
		hu[i] = human(name, addr, location);
	}

	cout << "name " << hu[2].name << endl;
	cout << "addr " << hu[2].addr << endl;
	cout << "city " << hu[2].location << endl;

	return 0;
}