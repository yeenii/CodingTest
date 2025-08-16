#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include<iostream>

using namespace std;

const int n_max = 100;
const int m_max = 100;

int n, m;
int grid[n_max][m_max];
int distGrid[n_max][m_max];
int visited[n_max][m_max];

//하, 우
int dr[2] = {1, 0};
int dc[2] = {0, 1};

bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < m;
}
void dfs(int x, int y, int val)
{
	visited[x][y] = true; //방문여부
	distGrid[x][y] = val + 1; //거리 값

	for (int i = 0; i< 2; i++)
	{
		int nr = x + dr[i];
		int nc = y + dc[i];

		if (!inRange(nr, nc))
			continue;

		if (visited[nr][nc])
			continue;

		if (grid[nr][nc] == 0)
			continue;

		dfs(nr, nc, distGrid[x][y]);
	}
}

int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n >> m;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> grid[i][j];
			visited[i][j] = false;
			distGrid[i][j] = -1; //거리 값 -1로 초기화 
			//cout << grid[i][j] << " ";
		}
		//cout << endl;
	}

	//DFS
	dfs(0,0,-1); //첫좌표, 거리 값

	if (distGrid[n - 1][m - 1] != -1)
		cout << 1 << endl;
	else if (distGrid[n - 1][m - 1] == -1)
		cout << 0 << endl;

	//for (int i = 0; i < n; i++)
	//{
	//	for (int j = 0; j < m; j++)
	//	{
	//		cout << distGrid[i][j] <<" ";
	//		//cout << grid[i][j] << " ";
	//	}
	//	cout << endl;
	//	//cout << endl;
	//}

	return 0;
}
