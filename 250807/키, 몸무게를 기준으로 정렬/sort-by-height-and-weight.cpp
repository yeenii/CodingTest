#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>
#include<algorithm>

using namespace std;

const int n_max=10;
int n;

class Human {
public: 
	string name;
	int height;
	int weight;

	Human(string name, int height, int weight) {
		this->name = name;
		this->height = height;
		this->weight = weight;
	};

	Human() {};
};

Human human[n_max]; //객체 선언 및 리스트 생성

bool compare(const Human &a, const Human &b)
{
	if (a.height == b.height) //키가 같으면, 몸무게 내림차순
		return a.weight > b.weight;
		
	return a.height < b.height; //키 오름차순
}

int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n;

	//입력
	for (int i = 0; i < n; i++)
	{
		cin >> human[i].name >> human[i].height >> human[i].weight;
	}
	
	//정렬
	sort(human, human+n, compare);

	//출력
	for (int i = 0; i < n; i++)
	{
		cout << human[i].name <<" "<< human[i].height << " " << human[i].weight <<endl;
	}

	return 0;
}