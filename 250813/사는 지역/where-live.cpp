#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include<algorithm>

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

bool compare(const human &a, const human &b)
{
	if (a.name != b.name)
		return a.name > b.name; //내림차순
}
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

	//이름이 사전순으로 가장 느린 사람
	sort(hu, hu + n, compare); //객체 리스트 정렬

	cout << "name " << hu[0].name << endl;
	cout << "addr " << hu[0].addr << endl;
	cout << "city " << hu[0].location << endl;

	return 0;
}