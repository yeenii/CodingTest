#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>

using namespace std;

const int n_max = 100;
int n;
int r, c;

int grid[n_max][n_max];
int visited[n_max][n_max];

//상하좌우
int dr[4] = {-1,1, 0, 0};
int dc[4] = {0, 0, -1, 1};


bool inRange(int r, int c)
{
	return r >= 0 && r < n && c >= 0 && c < n;
}

void move(int r, int c)
{
	cout << grid[r][c] << " "; //출력

	for (int i = 0; i < 4; i++)
	{
		int nx = r + dr[i];
		int ny = c + dc[i];

		if (!inRange(nx, ny))
			continue;

		if (visited[nx][ny])
			continue;

		if (grid[r][c] < grid[nx][ny]) //현재 좌표보다 주변 좌표가 값이 크다면
		{
			r = nx;
			c = ny;
			visited[nx][ny] = true;
			move(nx, ny);
			break;
		}
		else
			continue;	

	}
}

int main(int argc, char** argv)
{

	//freopen("input.txt", "r", stdin);
	
	cin >> n >> r >> c;

	//좌표 0부터 시작함
	r -= 1;
	c -= 1;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> grid[i][j];
			visited[i][j] = false;
		}
	}

	visited[r][c] = true; //첫좌표 방문처리
	move(r, c); //주변 큰 숫자 구하기

	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}