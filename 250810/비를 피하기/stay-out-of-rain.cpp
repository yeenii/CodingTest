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
bool visited[n_max][n_max];

vector<pair<int, int>> humanPos;
vector<pair<int, int>> exitPos;

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

queue<pair<int, int>> que;

bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < n;
}

void bfs()
{
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

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			visited[i][j] = false;
		}
	}

	//거리 구하는 그리드 -1로 초기화
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			disGrid[i][j] = -1;
		}
	}

	//2. bfs로 최단 거리 구하기
	/* <<큐의 원리를 이해하지 못해 생긴 대참사>>
	* 출발 위치인 '비를 피하는 곳' 3개를 한번에 0으로 처리하고, 큐에 저장
	* 큐가 pop하면서 '비를 피하는 곳' 3개가 저장된 뒤에 주변 셀을 저장함
	* 셀은 처음 거리 값이 저장될 때, visited가 true가 되니까, 
	* 나중에 거리 값을 비교할 필요 없이 맨 처음에 저장된 거리 값이 최단 거리 값이 됨
	* 사람이 없는 곳은 나중에 출력할 때, 0처리해주면 됨.
	*/
	// 1로는 이동 불가. 나머지는 가능
	for (int i = 0; i < m; i++)
	{
		int sr = exitPos[i].first;
		int sc = exitPos[i].second;

		que.push({ sr,sc });
		disGrid[sr][sc] = 0;
		visited[sr][sc] = true;
	}

	//비 피할 곳 부터 역순으로 구함
	bfs();

	//3. tempGrid에서 사람 아닌 셀은 0으로 만들고
	//결과 출력
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (grid[i][j] != 2) //사람이아니라면
				disGrid[i][j] = 0;

			cout << disGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	return 0;
}