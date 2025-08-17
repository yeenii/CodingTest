#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include<vector>
#include<climits>

using namespace std;

const int n_max = 20;
const int m_max = 40;

int n, m, t;
int grid[n_max][n_max];
int countTemp[n_max][n_max];
int countNum[n_max][n_max];

vector<pair<int, int>> beadPos;
bool isEnd[m_max];

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

bool inRange(int r, int c)
{
	return r >= 0 && r < n && c >= 0 && c < n;
}

void moveMax(int index, int r, int c)
{

	int maxNum = INT_MIN;
	int maxR = -1; 
	int maxC = -1;
	for (int i = 0; i < 4; i++)
	{
		int nr = r + dr[i];
		int nc = c + dc[i];

		if (!inRange(nr, nc))
			continue;

		if (maxNum < grid[nr][nc])
		{
			maxNum = grid[nr][nc];
			maxR = nr;
			maxC = nc;
		}
	}

	//위치 변경
	beadPos[index].first= maxR;
	beadPos[index].second = maxC;
	countNum[maxR][maxC]++;
	

}

int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n >> m >> t;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> grid[i][j];
		}
	}

	for (int i = 0; i < m; i++)
	{
		int r, c;
		cin >> r >> c;

		r = r - 1;
		c = c - 1;

		beadPos.push_back({r,c});
		isEnd[i] = false;
	}

	//이동
	for (int k = 0; k < t; k++) //t초 동안
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				countNum[i][j] = 0;
			}
		}

		//1. 주변에서 가장 큰 값으로 이동
		for (int i = 0; i < m; i++)
		{
			if (isEnd[i]) //충돌한 구슬은 건너뛰기
				continue;

			moveMax(i, beadPos[i].first, beadPos[i].second);
		}

		//2. 충돌한 구슬은 충돌 처리
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (countNum[i][j] > 1)
				{
					int deleteR = i;
					int deleteC = j;

					for (int i = 0; i < m; i++)
					{
						if (deleteR == beadPos[i].first && deleteC == beadPos[i].second)
						{
							isEnd[i] = true;
						}
					}
				}
			}
		}

		
	}
	
	int cnt = 0;
	for (int i = 0; i < m; i++)
	{
		if (!isEnd[i]) //충돌하지 않은 구슬의 개수
			cnt++;
	}

	cout << cnt << endl;
	


	return 0;
}