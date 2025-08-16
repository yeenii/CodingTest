#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

const int n_max = 30;

int n;

int grid[n_max][n_max];

//상하좌우
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};
int visited[n_max][n_max];

int villageCnt; //마을 개수
int cnt;


bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < n;
}

void dfs(int x, int y)
{
	visited[x][y] = true;
	cnt++;

	for (int i = 0; i < 4; i++)
	{
		int nx = x + dr[i];
		int ny = y + dc[i];

		if (!inRange(nx, ny))
			continue;

		if (visited[nx][ny])
			continue;

		if (grid[nx][ny] == 0) //벽
			continue;

		dfs(nx, ny);
	}

}

vector<int> humanCnt;
int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> grid[i][j];
			visited[i][j] = false;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			
			if (visited[i][j])
				continue;

			if (grid[i][j] == 0)
				continue;

			cnt = 0;
			dfs(i, j);

			humanCnt.push_back(cnt);
		}
	}

	//오름차순 정렬
	sort(humanCnt.begin(), humanCnt.end());

	//결과 출력
	cout << (int)humanCnt.size() << endl;
	for (auto h : humanCnt)
	{
		cout << h << endl;
	}

	return 0;
}