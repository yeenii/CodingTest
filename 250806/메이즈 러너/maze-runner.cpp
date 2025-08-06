#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

const int n_max = 15;
const int m_max = 15;
const int k_max = 100;

int n, m, k;
vector<vector<int>> miro;
vector<vector<int>> partic;
vector<vector<int>> partic_sum;
//vector<vector<int>> exitCell;
int exitRow, exitCol; //출구좌표

vector<vector<int>> tempPartic;
vector<vector<int>> tempMiro;

pair<int, int> parPos[m_max];//참가자
int sumMove; //참가자들의 이동거리 합

//상하좌우
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

int partic_live[m_max]; //참가자 생존 여부

bool inRange(int row, int col)
{
	return row >= 1 && row <= n && col >= 1 && col <= n;
}

void movePart(int t)
{
	/*cout << t << ": " << endl;
	for (int i = 1; i <= m; i++)
	{
		cout << parPos[i].first << " " << parPos[i].second << endl;
	}*/

	//1. 참가자 이동
	for (int i = 1; i <= m; i++)
	{
		if (!partic_live[i]) //** 참가자가 탈출한 경우 이동 못함
			continue;

		//참가자와 탈출구 간의 거리
		int minDist = abs(parPos[i].first - exitRow) + abs(parPos[i].second - exitCol);

		//인접칸
		int moveDir = -1; //움직이는 방향. 안 움직이면 -1
		for (int dir = 0; dir < 4; dir++)
		{
			int nextRow = parPos[i].first + dr[dir];
			int nextCol = parPos[i].second + dc[dir];

			//범위 밖이거나, 벽이 있는 경우
			if (!inRange(nextRow, nextCol) || miro[nextRow][nextCol] >= 1)
				continue;

			int dist = abs(nextRow - exitRow) + abs(nextCol - exitCol);

			if (minDist > dist)
			{
				moveDir = dir;
				break; //상하 우선이므로, 먼저나오는 방향 나오면 break.
			} //움직인 거리가 더 멀면 continue.

		}

		if (moveDir != -1) //참가자가 움직이면
		{
			//cout << t << ": " << i << ": " << parPos[i].first << " " << parPos[i].second << endl;

			parPos[i].first = parPos[i].first + dr[moveDir];
			parPos[i].second = parPos[i].second + dc[moveDir];
			sumMove += 1; //참가자 이동 횟수

			if (exitRow == parPos[i].first && exitCol == parPos[i].second) //참가자가 탈출구에 간 경우
			{
				partic_live[i] = false;
				parPos[i].first = -1;
				parPos[i].second = -1;
			}
				

			//cout << t <<": " << i << ": " << parPos[i].first << " " << parPos[i].second << endl;
			
		}

	}

	
}

//참가자 누적합
void prefix_sum()
{
	//참가자 표시용 그리드 초기화
	partic.assign(n + 1, vector<int>(n + 1, 0)); //참가자 위치 초기화
	partic_sum.assign(n + 1, vector<int>(n + 1, 0)); //참가자 위치 누적합 초기화

	//참가자 표시
	for (int i = 1; i <= m; i++)
	{
		if (partic_live[i] == false)
		{
			//cout << i << " 탈출함" << endl;
			continue;
		}

		//cout << "i: " << i << " x:" << parPos[i].first << " y:" << parPos[i].second << endl;
		partic[parPos[i].first][parPos[i].second] += 1; //한칸에 여러명 있는 참가자 표시위해 +=1
	}

	//참가자 누적합
	for (int p = 1; p <= m; p++)
	{
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				partic_sum[i][j] = partic_sum[i - 1][j] + partic_sum[i][j - 1] - partic_sum[i - 1][j - 1]+partic[i][j];
				
			}
		}
	}
	
}

//정사각형 내 참가자 수 조회 
bool num_partic(int sr, int sc, int er, int ec)
{

	//구간합으로 참가자 수 조회
	int num = partic_sum[er][ec] - partic_sum[sr - 1][ec] - partic_sum[er][sc - 1] + partic_sum[sr - 1][sc - 1];

	if (num >= 1)//참가자 수 1명 이상인 경우
		return true;

	return false;
}

bool exit_here(int sr, int sc, int er, int ec)
{
	bool exitCnt = false;
	if (sr <= exitRow && sc <= exitCol && er >= exitRow && ec >= exitCol) //탈출구 있는지
		exitCnt = true;

	return exitCnt;
}

void RotateSquare(int sr, int sc, int h)
{

	tempMiro.assign(h, vector<int>(h, 0)); //참가자 temp
	tempPartic.assign(h, vector<int>(h, 0)); //미로 temp

	//cout << "parPos: " << parPos[2].first << " " << parPos[2].second << endl;
	/*cout << "회전 전" << endl;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cout << partic[i][j] << " ";
		}
		cout << endl;
	}*/
	
	//1. 벽 90도 회전 & 벽 내구성 -1
	//3. 참가자 90도 회전
	for (int i = sr; i < sr+h; i++)
	{
		for (int j = sc; j < sc+h; j++)
		{
			/*
			- 90도 회전
			- (sr,sc) : 정사각형 왼쪽 위 좌표
			- h: 한 변의 길이
			- (i,j): 원래 격자에서 현재 좌표
			- 왼쪽 위 좌표 (0,0) 기준으로 변환
			*/
			
			int x = i - sr;
			int y = j - sc;

			int newX, newY; //회전한 위치
			newX = y;
			newY = h - 1 - x;

			
			tempMiro[newX][newY] = miro[i][j]; //벽 회전
			if (miro[i][j] >= 1) //내구성 -1
				tempMiro[newX][newY] -= 1;

			tempPartic[newX][newY] = partic[i][j]; //참가자 회전
		}
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < h; j++)
		{
			miro[sr + i][sc + j] = tempMiro[i][j]; //**sc를 sr로 써서 틀림
			partic[sr+i][sc+j] = tempPartic[i][j];
			//cout << "sr: " << sr + i << " sc: " << sc + j << endl;
		}
	}
	
	//**parPos 갱신
	//참가자가 탈출했다면 갱신 안함
	for (int i = 1; i <= m; i++)
	{
		if (!partic_live[i])
			continue;

		// 회전 전 좌표 → 로컬 좌표 변환
		int x = parPos[i].first - sr;
		int y = parPos[i].second - sc;

		if (x >= 0 && x < h && y >= 0 && y < h)
		{
			// 90도 회전
			int newX = y;
			int newY = h - 1 - x;

			// 다시 전역 좌표로 변환
			parPos[i].first = sr + newX;
			parPos[i].second = sc + newY;
		}
	}


	/*cout << "회전 후" << endl;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cout << partic[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	

	/*for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < h; j++)
		{
			cout << tempPartic[i][j] << " ";
		}
		cout << endl;
	}*/

	
	
}

void RotateExit(int sr, int sc, int h)
{
	//cout << "sc: " << sr << " sc: " << sc << " h: " << h << endl;
	int x = exitRow - sr;
	int y = exitCol - sc;

	int newX = y;
	int newY = h - 1 - x;

	//90도 회전한 위치로 갱신
	exitRow = sr + newX;
	exitCol = sc + newY;

	//cout << "---exit" << endl;
	//cout << exitRow << " " << exitCol << endl;

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

		miro.assign(n+1, vector<int>(n+1,0)); //미로 1-based 초기화

		//미로 입력
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				cin >> miro[i][j];
			}
		}

		//참가자들의 위치 리스트 저장
		for (int i = 1; i <= m; i++)
		{
			int r, c;
			cin >> r >> c;
			parPos[i].first = r;
			parPos[i].second = c;
			partic_live[i] = true;
		}

		//출구 좌표 입력
		cin >> exitRow >> exitCol;

		sumMove = 0; //참가자 총 이동거리 합 초기화

		for (int t = 1; t <= k; t++) //k초
		{
			//참가자 이동
			movePart(t);
			
			//2. 미로 회전
			//(1) 미로에서 누적합으로 참가자와 탈출구 개수 구함
			prefix_sum();
			//(2) 정사각형 구하기
			bool selectSquare = false;
			for (int h = 2; h <=n; h++) //가능한 정사각형 2x2, 3x3, 4x4, 5x5
			{
				for (int sr = 1; sr <= n-h+1; sr++)
				{
					for (int sc = 1; sc<= n - h+1; sc++)
					{
						//우하단좌표
						int er = sr + h-1;
						int ec = sc + h-1;

						if (!inRange(sr, sc) || !inRange(er, ec))
							continue;

						bool particCnt = num_partic(sr,sc,er,ec); //참가자수 리턴

						bool exitCnt = exit_here(sr,sc,er,ec);//탈출구 여부

						if (exitCnt && particCnt) //탈출구가 있고, 참가자가 1명 이상
						{
							RotateSquare(sr, sc, h);//선택된 정사각형 90도 회전 & 내구성-1
							RotateExit(sr, sc, h); //탈출구 90도 회전
							
							selectSquare = true; //r좌표와 c좌표가 작은 것이 우선임
							break;
						}

						
					}
					if (selectSquare)
						break;
				}
				if (selectSquare)
					break;
			}

			
		}

		//결과 출력
		cout << sumMove << endl;
		cout << exitRow << " " << exitCol << endl;
		
	//}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}