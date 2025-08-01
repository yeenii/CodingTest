#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>

using namespace std;

const int n_max = 10;
const int m_max = 100;

int n, m;
pair<int, int> pos[m_max];
int grid[n_max][n_max];

int main()
{
	//freopen("input.txt", "r", stdin);

	cin >> n >> m;

	for (int i = 0; i < m; i++)
	{
		cin >> pos[i].first >> pos[i].second;
		pos[i].first -= 1; //격자 0부터 시작
		pos[i].second -= 1;
		
	}

	//격자 초기화
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			grid[i][j] = 0;
		}
	}

	//격자에 동전의 위치 입력
	for (int i = 0; i < m; i++)
	{
		grid[pos[i].first][pos[i].second] = 1;
	}

	//출력
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << grid[i][j]<<" ";
		}
		cout << endl;
	}


	return 0;
}