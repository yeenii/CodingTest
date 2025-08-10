#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<climits>
#include<algorithm>

using namespace std;

const int n_max = 100;
const int h_max = 200;

int n, m, h;
int grid[n_max][n_max]; //입력
int disGrid[n_max][n_max]; //'비를 피할 수 있는 위치'부터 최단 거리 구하기
int tempGrid[n_max][n_max]; //사람의 최단 거리만 저장
bool visited[n_max][n_max];

vector<pair<int, int>> humanPos;
vector<pair<int, int>> exitPos;

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};


bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < n;
}

void bfs(int sr, int sc, int val)
{
	queue<pair<int, int>> que;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			visited[i][j] = false;
		}
	}

	que.push({sr,sc});
	disGrid[sr][sc] = 0;
	visited[sr][sc] = true;

	while (!que.empty())
	{
		int currentRow = que.front().first;
		int currentCol = que.front().second;
		que.pop();

		for (int dir = 0; dir < 4; dir++)
		{
			int nextRow = currentRow + dr[dir];
			int nextCol = currentCol + dc[dir];

			if (!inRange(nextRow, nextCol))
				continue;

			if (visited[nextRow][nextCol])
				continue;

			if (grid[nextRow][nextCol] == 1) //벽
				continue;

			que.push({nextRow, nextCol});
			visited[nextRow][nextCol] = true;
			disGrid[nextRow][nextCol] = disGrid[currentRow][currentCol] + 1;
		}

	}

}

int main()
{
	//freopen("input.txt", "r", stdin);

	//입력
	cin >> n >> h >> m;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> grid[i][j];
		}
	}

	//1. 2, 3의 위치 구하기 
	//사람 위치 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (grid[i][j] == 2)
				humanPos.push_back({i,j});

			if (grid[i][j] == 3)
			{
				exitPos.push_back({ i,j });
			}
				
		}
		
	}

	//tempGrid : 사람의 최단거리만 저장하는 2차원 격자
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			tempGrid[i][j]=INT_MAX;

		}

	}


	//2. bfs로 최단 거리 구하기
	// 1로는 이동 불가. 나머지는 가능
	for (int i = 0; i < m; i++)
	{
		//거리 구하는 그리드 -1로 초기화
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				disGrid[i][j] = -1;
			}
		}

		//비 피할 곳 부터 역순으로 구함
		bfs(exitPos[i].first, exitPos[i].second, -1);//시작 좌표, 거리값

		//사람의 최단 거리 구하기
		for (int j = 0; j < h; j++)
		{
			
			tempGrid[humanPos[j].first][humanPos[j].second] 
				= min(tempGrid[humanPos[j].first][humanPos[j].second], disGrid[humanPos[j].first][humanPos[j].second]);

		}


	}

	//3. tempGrid에서 사람 아닌 셀은 0으로 만들고
	//결과 출력
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (grid[i][j]!=2) //사람이아니라면
				tempGrid[i][j] = 0;
			else if (!visited[i][j]) //사람이지만, 방문하지 않았다면
				tempGrid[i][j] = -1;

			cout << tempGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	return 0;
}