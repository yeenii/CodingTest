#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>

using namespace std;

const int n_max = 100;
const int k_max = 100;

int n, k;
int grid[n_max][n_max];
int distGrid[n_max][n_max];
bool visited[n_max][n_max];

vector<pair<int, int>> badOrg; //상한귤
vector<pair<int, int>> org; //귤

queue<pair<int, int>> que;

//상하좌우
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

bool inRange(int x, int y)
{
	return  x >= 0 && x < n && y >= 0 && y < n;
}
void bfs()
{
	while (!que.empty())
	{
		int cr = que.front().first;
		int cc = que.front().second;
		que.pop();

		for (int i = 0; i < 4; i++)
		{
			int nr = cr + dr[i];
			int nc = cc + dc[i];

			if (!inRange(nr, nc))
				continue;

			if (visited[nr][nc])
				continue;

			if (grid[nr][nc] == 0)
				continue;

			que.push({nr,nc});
			visited[nr][nc]=true;
			distGrid[nr][nc] = distGrid[cr][cc] + 1;
		}
		
	}
}

int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n >> k;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> grid[i][j];
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (grid[i][j] == 2)
				badOrg.push_back({i,j});
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			distGrid[i][j] = -2;
			visited[i][j] = false;
		}
	}

	//상한귤 개수만큼 큐에 저장하고, 0 값으로 저장
	for (int i = 0; i < k; i++)
	{
		int sr = badOrg[i].first;
		int sc = badOrg[i].second;

		que.push({sr, sc});
		distGrid[sr][sc] = 0;
		visited[sr][sc] = true;
	}

	bfs(); //상한귤 -> 귤 최단거리

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (grid[i][j] == 0)
				distGrid[i][j] = -1;

			cout << distGrid[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}