#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include<iostream>
#include<climits>
#include<vector>
#include<algorithm>

using namespace std;

const int n_max = 50;
const int m_max = 50;
const int k_max = 100;

int n, m;
int grid[n_max][m_max];
int visited[n_max][m_max];

int cnt;

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

//int safeNum[k_max];
vector<pair<int,int>> safeNum; //k마다 안전지대 개수

bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < m;
}

void dfs(int x, int y, int k)
{
	visited[x][y] = true;

	for (int i = 0; i < 4; i++)
	{
		int nr = x + dr[i];
		int nc = y + dc[i];

		if (!inRange(nr, nc))

			continue;
		if (visited[nr][nc])
			continue;

		if (grid[nr][nc] <= k)
			continue;

		dfs(nr, nc, k);
	}
}

bool compare(pair<int,int> &a, pair<int,int> b)
{

	if (a.second == b.second) //안전지대 개수가 같으면
		return a.first < b.first; //k가 작은 것

	return a.second > b.second; //안전지대는 내림차순
}
int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n >> m;

	int maxNum = INT_MIN;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> grid[i][j];

			if (maxNum < grid[i][j])
				maxNum = grid[i][j];
		}
	}

	for (int k = 1; k <= maxNum; k++)
	{
		cnt = 0;
		//k마다 visited 초기화
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				visited[i][j] = false;
			}
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (visited[i][j])
					continue;

				if (grid[i][j] <= k)
					continue;


				dfs(i, j, k);
				cnt++;


			}
		}
		safeNum.push_back({k, cnt});
	}
	
	sort(safeNum.begin(), safeNum.end(), compare);

	/*for (auto s : safeNum)
	{
		cout << s.first << " " << s.second << endl;
	}*/

	cout << safeNum[0].first << " " << safeNum[0].second << endl;

	return 0;
}