#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include<algorithm>

using namespace std;
const int n_max = 100;
int n;

int dr[4] = {0, 1, -1, 0}; //서남북동
int dc[4] = {-1, 0, 0, 1};

int x = 0, y = 0;
int cnt;

bool canGo=false;

void move(int num, int dir_num)
{
	while (num > 0)
	{
		num--;
		cnt++; //시작점까지 돌아오는데 걸린 시간

		int nx = x + dr[dir_num]; //정해진 방향으로 이동
		int ny = y + dc[dir_num];

		//x, y 갱신
		x = nx;
		y = ny;

		//x,y가 시작점으로 돌아왔다면
		if (x == 0 && y == 0)
		{
			cout << cnt << endl;
			canGo = true;
			break;
		}
	}
}

int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n;

	while(n>0)
	{		
		char dir;
		int num;
		cin >> dir >> num;

		if (dir == 'N')
		{
			move(num, 2);
			
		}
		else if (dir == 'E')
		{
			move(num, 3);
		}
		else if (dir == 'S')
		{
			move(num, 1);
		}
		else if (dir == 'W')
		{
			move(num, 0);
		}

		if (canGo) //시작점으로 돌아왔다면, n 줄어드는 것 멈추기 ** 그래야 -1 출력
			break;

		n--;

	}

	//시작점으로 돌아오지 못한 경우
	if (n == 0 && x != 0 && y != 0)
		cout << -1 << endl;

	return 0;
}