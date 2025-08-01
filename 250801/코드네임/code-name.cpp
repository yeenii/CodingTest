#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>
#include<climits>

using namespace std;

class agent
{
public:
	char name; //이름
	int point; //점수

	//생성자
	agent(char name, int point)
	{
		this->name = name;
		this->point = point;
	}

	//'agent': 사용할 수 있는 적절한 기본 생성자가 없습니다. 라고 뜨면 
	// 매개변수 초기화 하거나, 인자값이 전혀 넘어오지 않는 생성자를 하나더 정의 해야 함
	agent() {}
};

agent Agent[5]; //리스트 생성

int main()
{
	//freopen("input.txt", "r", stdin);

	for (int i = 0; i < 5; i++)
	{
		char name;
		int point;

		cin >> name >> point;
		Agent[i] = agent(name, point); //객체 생성 및 리스트에 추가
	}

	int minPoint = INT_MAX;
	for (int i = 0; i < 5; i++)
	{
		if (Agent[i].point < minPoint)
			minPoint = Agent[i].point;
	}

	//출력
	for (int i = 0; i < 5; i++)
	{
		if (minPoint == Agent[i].point)
			cout << Agent[i].name << " " << Agent[i].point << endl;
	}
	

	return 0; 
}