#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<queue>

using namespace std;

const int n_max = 100;
const int m_max = 100;
int n, m;

int grid[n_max][m_max];

//상하좌우
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

bool visited[n_max][m_max];



bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < m;
}

void bfs(int sr, int sc, int n)
{
	queue<pair<int, int>> que;

	que.push({sr, sc});
	visited[sr][sc] = true;
	grid[sr][sc] = 0;

	while (!que.empty())
	{
		int currentRow = que.front().first;
		int currentCol = que.front().second;
		que.pop();

		for (int dir = 0; dir < 4; dir++)
		{
			int nextRow = currentRow + dr[dir];
			int nextCol = currentCol + dc[dir];

			if (visited[nextRow][nextCol]) //방문 여부
				continue;

			if (!inRange(nextRow, nextCol) || grid[nextRow][nextCol] == 0) //범위 밖이거나 뱀이 있는 경우
				continue;

			que.push({nextRow, nextCol});
			visited[nextRow][nextCol] = true;
			grid[nextRow][nextCol] = grid[currentRow][currentCol]+1;
			
		}
	}
	
	
}

int main()
{
	//int test_case;
	//int T;

	//freopen("input.txt", "r", stdin);

	//cin >> T;

	//for (test_case = 1; test_case <= T; ++test_case)
	//{
		cin >> n >> m;

		for (int i = 0; i < n ; i++)
		{
			for (int j = 0; j < m; j++)
			{
				cin >> grid[i][j];
			}
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if(grid[i][j]==1)
					grid[i][j]=-1;
			}
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				visited[i][j] = false;
			}
		}

		//최단 거리 구하기 
		bfs(0, 0, -1); //시작 좌표 x, y, 거리 크기
	

		if (grid[n-1][m-1] == -1) //최종 위치가 -1인 경우 = 움직일 수 없는 경우
		{
			cout << -1 << endl; //-1출력하고 종료
		}
		else {
			//탈출 위치까지 최단 거리 출력
			cout << grid[n - 1][m - 1] << endl;
		}
		
		
	//}

	return 0;
}