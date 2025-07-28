#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include <cstdio>
using namespace std;

const int n_max = 105;
const int m_max = 110;

int n, m, k;
int space[n_max][n_max];

int canGo() //다음 행으로 이동 가능한지
{
	for (int i = 1; i <= n-1; i++)
	{
		for (int j = k; j <= k + m - 1; j++)
		{
			if (space[i + 1][j] == 1) //그 다음 행이 1인 경우
			{
				return i; //현재 행 리턴 후, break
				break;
			}

			//그 다음 행이 0인 경우, 다음 열도 다음 행이 막혀있는지 확인
		}
	}

	return n; //끝까지 내려갈 수 있는 경우

}

int main(int argc, char** argv)
{
	//int test_case;
	//int T;

	//freopen("input.txt", "r", stdin);
	//cin >> T;
	
	//for (test_case = 1; test_case <= T; ++test_case)
	//{
		cin >> n >> m >> k;

		for (int i = 1; i <=n; i++)
		{
			for (int j = 1; j<= n; j++)
			{
				cin >> space[i][j];
			}
		}

		int new_row = canGo();

		for (int col = k; col <= k + m + 1; col++)
		{
			space[new_row][col] = 1;
		}
		
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				cout << space[i][j] << " ";
			}
			cout << endl;
		}

	//}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}