#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>
#include<queue>
#include<algorithm> //max()

using namespace std;

const int R_MAX = 80; //행
const int C_MAX = 80; //열
const int k_MAX = 1005; //정령의 수 

int R, C, K;
int c, d;

int forest[R_MAX + 3][C_MAX]; //숲. 
int isExit[R_MAX + 3][C_MAX]; // 출구만 기록

//방향 
int dr[4] = { -1,0, 1, 0 };
int dc[4] = { 0, 1, 0 , -1 };

int result;
int visited[R_MAX + 3][C_MAX];

bool inRange(int r, int c)
{
	if (r >= 3 && r < R + 3 && c >= 0 && c < C)
		return true;
	else
		return false;
}

bool canGo(int r, int c)
{
	bool canDo = r + 1 < R + 3; //**
	canDo = canDo && c - 1 >= 0;
	canDo = canDo && c + 1 < C;

	canDo = canDo && (forest[r - 1][c - 1] == 0);
	canDo = canDo && (forest[r - 1][c] == 0);
	canDo = canDo && (forest[r - 1][c + 1] == 0);
	canDo = canDo && (forest[r][c - 1] == 0);
	canDo = canDo && (forest[r][c] == 0);
	canDo = canDo && (forest[r][c + 1] == 0);
	canDo = canDo && (forest[r + 1][c] == 0);

	//cout << canDo << endl;
	return canDo;

}

int bfs(int r, int c)
{
	queue<pair<int, int>> que;
	int maxRow = r; // 현재 행을 최대 행으로 초기화

	//방문 처리 
	for (int i = 0; i < R + 3; i++)
	{
		for (int j = 0; j < C; j++)
		{
			visited[i][j] = false;
		}
	}

	//1. 큐에 푸시하면서 방문 처리
	que.push({ r,c });
	visited[r][c] = true;

	//2. 큐가 비어있지 않다면
	while (!que.empty())
	{
		//3. 큐를 팝
		int currentRow = que.front().first;
		int currentCol = que.front().second;
		que.pop();

		//4. 팝한 것과 인접한 것 비교 
		for (int i = 0; i < 4; i++)
		{
			int nextRow = currentRow + dr[i];
			int nextCol = currentCol + dc[i];

			if (!inRange(nextRow, nextCol))
				continue;

			if (visited[nextRow][nextCol])
				continue;

			if (forest[nextRow][nextCol] == forest[currentRow][currentCol] ||
				(isExit[currentRow][currentCol] == 1 && forest[nextRow][nextCol] != 0))
			{
				que.push({ nextRow, nextCol });
				visited[nextRow][nextCol] = true;

				maxRow = max(maxRow, nextRow);
				
			}


		}

	}

	//5. 인접하다면 큐에 푸시하면서 방문 처리 

	return maxRow;
}

void move(int index, int r, int c, int d)//열, 방향
{
	//1. 남쪽으로 한칸
	if (canGo(r + 1, c))
	{
		move(index, r + 1, c, d);
	}
	else if (canGo(r + 1, c - 1)) //2. 서쪽 방향으로 회전(반시계)하면서 한칸 - 아래로 한칸
	{
		move(index, r + 1, c - 1, (d + 3) % 4);
	}
	else if (canGo(r + 1, c + 1)) //3. 동쪽 방향으로 회전(시계)하면서 한칸 - 아래로 한칸
	{
		move(index, r + 1, c + 1, (d + 1) % 4);
	}
	else //최대한 남쪽으로 간 경우
	{
		//숲 범위 벗어난 경우
		if (!inRange(r - 1, c - 1) && !inRange(r - 1, c + 1))
		{
			for (int i = 0; i < R + 3; i++)
			{
				for (int j = 0; j < C; j++)
				{
					forest[i][j] = 0;
					isExit[i][j] = 0;
				}
			}
		}
		else //숲 범위 벗어나지 않아, 정령 이동 
		{

			//숲에 골렘 정착
			forest[r][c] = index;
			for (int i = 0; i < 4; i++)
			{
				forest[r + dr[i]][c + dc[i]] = index;
			}

			/*for (int i = 0; i < R + 3; i++)
			{
				for (int j = 0; j < C; j++)
				{
					cout << forest[i][j] << " ";
				}
				cout << endl;

			}
			cout << endl;*/

			//최종 출구 
			isExit[r + dr[d]][c + dc[d]] = 1;

			/*for (int i = 0; i < R + 3; i++)
			{
				for (int j = 0; j < C; j++)
				{
					cout << isExit[i][j] << " ";
				}
				cout << endl;

			}
			cout << endl;*/

			result += bfs(r, c) - 3 + 1; //-3+1 해야지 마지막 행이 6이 됨
			//cout << result << endl;

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

		//cout << "#" << test_case << ": " << endl;

		//[문제 코드 작성]
		/*
		- R행 x C열 (가장 위 1행, 가장 아래 R행)
		- 북을 통해서만 들어올 수 있음
		- K명의 정령은 각자 골렘을 탐

		[골렘]
		- 십자 모양 구조 (중앙칸 포함 5칸)
		- 골렘의 출구 : 골렘 중앙 제외 4칸 중 하나
		- 어느 방향에서든 탑승 가능
		- 내릴 때는 정해진 출구에서만

		[골렘 이동]
		- i번째로 숲을 탐색하는 골렘
		- 가장 위 밖에서 시작
		- 골렘의 중앙이 ci 열이 되도록 하는 위치에서 내려옴
		- 초기 골렘의 출구 : di 방향 (0~3. 북(0)-동(1)-남(2)-서(3))

		[우선순위]
		1. 남쪽으로 한칸
		2. 서쪽 방향으로 회전(반시계)하면서 한칸 - 아래로 한칸
		3. 동쪽 방향으로 회전(시계)하면서 한칸 - 아래로 한칸
		4. 최대한 남쪽으로 이동했지만, 골렘의 몸 일부가 숲에 벗어난 경우 -> 새롭게 숲 탐색

		최대한 남쪽으로 이동해, 이동할 수 없는 경우

		[정령 이동]
		- 정령은 골렘 내 상하좌우 인접 칸으로 이동
		- 현재 위치 하고 있는 골렘의 출구가 다른 골렘과 인접하다면,
		  출구를 통해 다른 골렘으로 이동
		- 최대한 남쪽 칸으로 이동 - 최종 행 위치 누적
		- 정령의 최종 위치의 행 번호 합을 구함

		*/

		cin >> R >> C >> K; //숲의 크기 RxC, 정령의 수 K
		//cout << R << " " << C << " " << K << endl;

		//숲 초기화 
		for (int i = 0; i < R + 3; i++)
		{
			for (int j = 0; j < C; j++)
			{
				forest[i][j] = 0;
				isExit[i][j] = 0;
			}

		}

		for (int i = 1; i <= K; i++) //1~K번
		{
			cin >> c >> d; //열, 방향
			//cout << c <<" "<< d <<endl;

			move(i, 1, c - 1, d); //인덱스, 행, 열, 방향

		}

		cout << result << endl;

	//}
	return 0;
}