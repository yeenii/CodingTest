#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>

using namespace std;

//서남북동
int dx[4] = {-1, 0,0, 1 };
int dy[4] = {0,-1, 1, 0};

int n;

//초기값
int x;
int y;

void moveDir(int dir, int num)
{
	//움직일 거리 만큼 이동
	for (int i = 0; i < num; i++)
	{
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		x = nx;
		y = ny;
	}

}

int main(int argc, char** argv)
{

	//freopen("input.txt", "r", stdin);
	
	cin >> n;

	x = 0, y = 0;

	//n개의 줄에 거려 이동방향, 이동 거리
	for (int i = 0; i < n; i++)
	{
		char dir;
		int num;
		int dir_num = -1;
		cin >> dir >> num;

		if (dir == 'W')
		{
			dir_num = 0;
			moveDir(dir_num, num);
		}
		else  if (dir == 'S')
		{
			dir_num = 1;
			moveDir(dir_num, num);
		}
		else if (dir == 'N')
		{
			dir_num = 2;
			moveDir(dir_num, num);
		}
		else if (dir == 'E')
		{
			dir_num = 3;
			moveDir(dir_num,num);
		}


	}

	//출력
	cout << x << " " << y << endl;
	

	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}