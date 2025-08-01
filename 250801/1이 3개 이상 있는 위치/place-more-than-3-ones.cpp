#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>

using namespace std;

const int n_max = 100;

int n;
int grid[n_max][n_max];

//상하좌우
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

bool inRange(int r, int c)
{
	return r >= 0 && r < n && c >= 0 && c < n;
}

//상하좌우로 인접한 칸 중 수사 1이 적혀있는 칸의 수가 3개 이상인 곳의 개수
int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> grid[i][j];
		}
	}

	int result = 0; //인접 칸 숫자 1이 3개 이상인 칸의 수 출력
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int cnt = 0;
			//현재 칸의 상하좌우
			for (int dir = 0; dir < 4; dir++)
			{
				int nx = i + dr[dir];
				int ny = j + dc[dir];

				if (inRange(nx, ny) && grid[nx][ny] == 1)
					cnt++;
			}

			if (cnt >= 3) //현재 칸의 인접 칸에 1이 3개 이상 적혀있는 경우
				result += 1;
			
		}
	}

	cout << result << endl;

	return 0;
}