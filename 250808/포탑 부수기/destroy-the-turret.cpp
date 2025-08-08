#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<climits>

using namespace std;

const int n_max = 15;
const int m_max = 15;
const int k_max = 1005;

int n, m, k;
int grid[n_max][m_max];

pair<int, int> atkerPos; //공격자 위치
pair<int, int> vctPos; //공격당하는자 위치

int gridBfs[n_max][m_max]; //bfs
bool visited[n_max][m_max]; //방문 여부


struct attacker {

	int hp; // 공격력
	int recentAttack; //가장 최근에 공격한 포탑 턴. 초기값 0. 마지막으로 공격자 선정된 턴 번호
	int sumRC; //행과 열의 합
	int col; //열
	int row;
};

vector<attacker> atker; //공격자 & 공격 당하는 자 

//우하좌상
int dr[8] = { 0, 1, 0, -1, -1, -1, 1, 1 };
int dc[8] = { 1, 0, -1, 0, -1, 1, -1, 1 };

vector<pair<int, int>> distancePos;
int applyAttack[n_max][m_max];

int lastAttacker[n_max][m_max]; //가장최근에 공격한 위치

void selectAttacker(int k) //공격자
{
	atker.clear(); //구조체 초기화

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (grid[i][j] == 0) //**포탄 없음은 건너뛰기
				continue;

			//공격력 낮음 - **가장 최근에 공격한 포탑 - 행열합 - 열이 가장 큰 포탑
			//lastAttacker[i][j]: 저장된 이전 순번이 저장됨
			atker.push_back({ grid[i][j], lastAttacker[i][j], i + j, j, i });
		}
	}

	sort(atker.begin(), atker.end(), [](const attacker& a, const attacker& b) {
		if (a.hp != b.hp) return a.hp < b.hp; //오름차순
		if (a.recentAttack != b.recentAttack) return a.recentAttack > b.recentAttack; //**내림차순
		if (a.sumRC != b.sumRC) return a.sumRC > b.sumRC; //내림차순
		if (a.col != b.col) return a.col > b.col; //내림차순
		});


	grid[atker[0].row][atker[0].col] += n + m; //공격자의 공격력 n+m만큼 증가
	//공격자 위치 저장
	atkerPos.first = atker[0].row;
	atkerPos.second = atker[0].col;

}

void selectAttack(int k) //공격당하는 자
{

	sort(atker.begin(), atker.end(), [](const attacker& a, const attacker& b) {
		if (a.hp != b.hp) return a.hp > b.hp; //내림차순
		if (a.recentAttack != b.recentAttack) return a.recentAttack < b.recentAttack; //**오름차순
		if (a.sumRC != b.sumRC) return a.sumRC < b.sumRC; //오름차순
		if (a.col != b.col) return a.col < b.col; //오름차순
		});


	//공격당하는자 위치 저장
	vctPos.first = atker[0].row;
	vctPos.second = atker[0].col;

	if (atkerPos.first == vctPos.first && atkerPos.second == vctPos.second) //공격자와 공격대상자 중복되는 경우, 다음 후보로
	{
		vctPos.first = atker[1].row;
		vctPos.second = atker[1].col;
	}
}

bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < m;
}

void bfs(int sr, int sc, int val)
{
	queue<pair<int, int>> que;

	que.push({ sr,sc });
	visited[sr][sc] = true;
	gridBfs[sr][sc] = 1;

	while (!que.empty())
	{
		int currentRow = que.front().first;
		int currentCol = que.front().second;
		que.pop();

		for (int i = 0; i < 4; i++)
		{
			int nextRow = (currentRow + dr[i] + n) % n; //**
			int nextCol = (currentCol + dc[i] + m) % m;

			if (!inRange(nextRow, nextCol))
				continue;

			if (visited[nextRow][nextCol])
				continue;

			if (gridBfs[nextRow][nextCol] == 0) //공격력 0인 경우
				continue;

			que.push({ nextRow, nextCol });
			visited[nextRow][nextCol] = true;
			gridBfs[nextRow][nextCol] = gridBfs[currentRow][currentCol] + 1; //역순

		}
	}

}



void attack(int k)
{
	//gridBfs 초기화
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (grid[i][j] != 0)
				gridBfs[i][j] = -1;
			else
				gridBfs[i][j] = 0;

		}
	}


	//방문여부 초기화
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{

			visited[i][j] = false;

		}
	}

	//공격 영향 여부 초기화
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (grid[i][j] == 0)
				applyAttack[i][j] = 0;
			else
				applyAttack[i][j] = -1; //초기화
		}
	}


	//최단 거리 계산
	bfs(vctPos.first, vctPos.second, -1); //첫 좌표, 값



	if (gridBfs[atkerPos.first][atkerPos.second] == -1) //포탄 공격
	{
		applyAttack[atkerPos.first][atkerPos.second] = 1; //공격자 영향

		//공격 대상은 공격자의 공격력만큼 피해 받음
		grid[vctPos.first][vctPos.second] -= grid[atkerPos.first][atkerPos.second];

		if (grid[vctPos.first][vctPos.second] < 0)
			grid[vctPos.first][vctPos.second] = 0;

		applyAttack[vctPos.first][vctPos.second] = 1;

		int currentRow = vctPos.first;
		int currentCol = vctPos.second;
		for (int i = 0; i < 8; i++)
		{
			int nextRow = (currentRow + dr[i] + n) % n;
			int nextCol = (currentCol + dc[i] + m) % m;

			if (!inRange(nextRow, nextCol))
				continue;

			if (grid[nextRow][nextCol] == 0) //부서진 포탑
				continue;

			//**공격자 건너뛰기
			//공격당하는자 바로 옆에 있으면, 공격자도 피해를 받음. 공격자 제외하고 다른 포탑만 피해가게 설정해야 함
			if (nextRow == atkerPos.first && nextCol == atkerPos.second)
				continue;

			//주변 8방향 셀 영향받음
			grid[nextRow][nextCol] = grid[nextRow][nextCol] - (grid[atkerPos.first][atkerPos.second] / 2);

			if (grid[nextRow][nextCol] < 0)
				grid[nextRow][nextCol] = 0;

			applyAttack[nextRow][nextCol] = 1;
		}
	}
	else
	{ //레이저 공격

		//최단 거리 이동
		int currentRow = atkerPos.first;
		int currentCol = atkerPos.second;
		while (!(currentRow == vctPos.first && currentCol == vctPos.second))
		{
			for (int i = 0; i < 4; i++)
			{
				int nextRow = (currentRow + dr[i] + n) % n; //**
				int nextCol = (currentCol + dc[i] + m) % m;

				if (!inRange(nextRow, nextCol))
					continue;

				if (gridBfs[nextRow][nextCol] == 0) //공격력 0인 경우 = 부서진 포탑 위치
					continue;


				if (gridBfs[currentRow][currentCol] - 1 == gridBfs[nextRow][nextCol])
				{
					currentRow = nextRow;
					currentCol = nextCol;
					distancePos.push_back({ currentRow, currentCol });
					break;
				}

			}
		}

		//공격력 수정
		applyAttack[atkerPos.first][atkerPos.second] = 1;
		for (auto d : distancePos)
		{
			//cout << d.first << " " << d.second << endl;
			applyAttack[d.first][d.second] = 1; //공격 영향받는 셀 저장

			if (!(d.first == vctPos.first && d.second == vctPos.second)) //가는 길에 위치한 셀
			{
				//공격자의 공격력 /2 만큼 뺌
				grid[d.first][d.second] = grid[d.first][d.second] - grid[atkerPos.first][atkerPos.second] / 2;

			}
			else {
				//공격 당하는 자 : 공격자의 공격력 만큼 뺌
				grid[d.first][d.second] -= grid[atkerPos.first][atkerPos.second];
			}

			if (grid[d.first][d.second] < 0) //0이하가 된 포탑은 0처리
				grid[d.first][d.second] = 0;
		}


		distancePos.clear(); //초기화
	}

	//가장 최근에 공격한 포탑 저장
	lastAttacker[atkerPos.first][atkerPos.second] = k;

	/*cout << "---" << k << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << grid[i][j] << " ";

		}
		cout << endl;
	}*/
}

void notAttack(int k)
{

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (applyAttack[i][j] != 1 && grid[i][j] != 0) //공격에 영향받지 않고, 부서진 포탑인 아닌 셀
			{
				grid[i][j] += 1; //공격과 무관한 포탑 공격력 1 상승
			}

		}
	}

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

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				cin >> grid[i][j];
			}
		}


		//**가장 최근에 공격한 포탑 초기화
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				lastAttacker[i][j] = 0; // 모든 포탑은 시점 0에서 모두 공격한 경험이 있다고 가정
			}
		}

		for (int t = 1; t <= k; t++)
		{

			//1. 공격자 선정
			selectAttacker(t);

			//2. 공격 대상자 선정 
			selectAttack(t);

			//3. 공격 (레이저 공격 -> 포탑 공격)
			attack(t);

			//4. 포탑 정비
			notAttack(t);

			

		}

		//가장 강한 포탑 출력
		int maxAtk = INT_MIN;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (grid[i][j] > maxAtk)
					maxAtk = grid[i][j];

			}
		}
		cout << maxAtk << endl;


	//}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}