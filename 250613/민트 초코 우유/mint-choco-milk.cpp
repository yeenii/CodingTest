#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include <cstdio>   // C++ 스타일에서 추천
#include<queue>
#include<vector>
#include<algorithm>

using namespace std;

const int N_MAX = 55;
const int T_MAX = 35;

int n, t;
char charFood[N_MAX][N_MAX];
int foodGrid[N_MAX][N_MAX];
int faithGrid[N_MAX][N_MAX];
int foodCount[N_MAX][N_MAX]; //기본 음식 조합 개수 카운트 
bool visited[N_MAX][N_MAX];

int dr[4] = {-1,1,0,0};
int dc[4] = { 0,0,-1,1 };

vector<pair<int, pair<int, int>>> group;
vector<pair<int, pair<int, pair<int, int>>>> leaderList;

bool defense[N_MAX][N_MAX]; //방어상태

int countBasicFood(int food)
{
	int cnt = 0;

	if (food & 1) //T(민트)
		cnt++;

	if (food & 2) //2(초코)
		cnt++;

	if (food & 4) //4(우유)
		cnt++;

	return cnt;
}

void morningCompute()
{

	//모든 학생 신앙심 1증가
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			faithGrid[i][j] += 1;
			//cout << faithGrid[i][j] << " ";
		}
		//cout << endl;
	}

}

int bfsGroup(int foodNum, int row, int col)
{
	queue<pair<int, int>> que;
	int groupCount = 1;

	que.push({row, col});
	visited[row][col] = true;
	group.push_back({ -faithGrid[row][col], {row,col} });


	while (!que.empty())
	{
		int currentRow = que.front().first;
		int currentCol = que.front().second;
		que.pop();

		for (int i = 0; i < 4; i++)
		{
			int nextRow = currentRow + dr[i];
			int nextCol = currentCol + dc[i];

			if (nextRow >= n || nextRow < 0 || nextCol >= n || nextCol < 0)
				continue;
			
			if (visited[nextRow][nextCol])
				continue;

			//현재와 인접 셀이 신봉 음식이 같다면
			if (foodGrid[nextRow][nextCol]== foodNum)
			{
				que.push({ nextRow, nextCol });
				visited[nextRow][nextCol] = true;
				group.push_back({ -faithGrid[nextRow][nextCol], {nextRow,nextCol} });
				groupCount++;
			}
		}
	}
	return groupCount;

}

void lunchCompute()
{
	//음식 조합 개수 세기
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int cnt = countBasicFood(foodGrid[i][j]); //**좌표가 아니라 각 셀의 숫자 넘겨주기
			foodCount[i][j] = cnt;

		}
	}
	
	//(1) 인접한 학생들과 신봉 음식이 같은 경우 그룹 형성
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			visited[i][j] = false;
		}

	}

	group.clear(); //그룹 저장 벡터 초기화
	leaderList.clear(); //대표자 리스트 벡터 초기화**

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (visited[i][j])
				continue;

			int count = bfsGroup(foodGrid[i][j], i, j);
			//cout << count << endl;

			sort(group.begin(), group.end());

			/*for (auto g : group)
			{
				cout << g.first << endl;
			}*/

			int leaderRow = group[0].second.first; //row
			int leaderCol = group[0].second.second; //row
			//int faith = group[0].first; //row
			//cout << faith << endl;
			//cout << leaderRow << " " << leaderCol << endl;

			faithGrid[leaderRow][leaderCol] += count - 1;
			for (int k = 1; k < group.size(); k++)
			{
				int gr = group[k].second.first;
				int gc = group[k].second.second;

				faithGrid[gr][gc] -= 1;

			}
			
			//대표들 - 조합음식개수 작은 순, 신앙심, 행, 열 순
			leaderList.push_back({ foodCount[leaderRow][leaderCol] ,
				{-faithGrid[leaderRow][leaderCol],{leaderRow, leaderCol}}});

			/*for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					cout << faithGrid[i][j] << " ";
				}
				cout << endl;
			}
			cout << endl;*/

			group.clear(); //초기화
		}

	} 

	sort(leaderList.begin(), leaderList.end());

	//for (int i = 0; i < leaderList.size(); i++)
	//{
	//	cout << leaderList[i].first<<endl; //조합음식개수 작은 순
	//	cout << leaderList[i].second.first<<endl; //신앙심 큰 순
	//	cout << leaderList[i].second.second.first << " " << leaderList[i].second.second.second << endl; //col 작은 순 
	//}
	
}

void FinalResult()
{
	int result[8] = {0};

	for (int k = 1; k <= 7; k++)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (foodGrid[i][j] == k)
					result[k] += faithGrid[i][j];
			}
		}

	}
	
	cout << result[7] << " " << result[3] << " " << result[5] << " " << result[6] << " " <<
		result[4] << " " << result[2] << " " << result[1] << endl;
}

void dinnerCompute()
{
	for (auto leader : leaderList) //우선순위 순 대표 전파 
	{
		int leaderR = leader.second.second.first;
		int leaderC = leader.second.second.second;
		//cout << "leaderR: " << leaderR << " " << "leaderC: " << leaderC << endl;

		if (defense[leaderR][leaderC]) //전파자가 방어상태이면, 전파하지 않음
			continue;

		int faith = -leader.second.first; //신앙심
		//cout << faith << endl;

		int x = faith - 1; // 간절함
		int dir = faith % 4; //전파 방향 
		//cout << dir << endl;
		faithGrid[leaderR][leaderC] = 1; //신앙심 1로 

		int currentRow = leaderR;
		int currentCol = leaderC;
		while (true)
		{
			//전파 방향으로 한칸씩 전파 
			//영역이 벗어나면 종료 
			currentRow += dr[dir];
			currentCol += dc[dir];
			//cout << currentRow << " " << currentCol << endl;

			if (currentRow >= n || currentRow < 0 || currentCol >= n || currentCol < 0) //영역 벗어나면 종료
			{
				//cout << "**" << endl;
				break;
			}
				
			if (x <= 0) //간절함이 0이되면 종료
				break;

			if (foodGrid[leaderR][leaderC] == foodGrid[currentRow][currentCol]) //신앙 음식이 같은 경우
			{
				continue;
			}
			else if (foodGrid[leaderR][leaderC] != foodGrid[currentRow][currentCol]) //신앙 음식이 다른 경우
			{
				int y = faithGrid[currentRow][currentCol];
				if (x > y) //강한 전파 
				{
					//전파자와 동일한 음식 선봉
					foodGrid[currentRow][currentCol] =foodGrid[leaderR][leaderC];

					//전파자 간절함 y+1 깎임
					x -= y + 1;
					//전파 대상자 신앙심 +1
					faithGrid[currentRow][currentCol] += 1;

					defense[currentRow][currentCol] = true; //방어상태 

					//간절함 0이되면 종료 
					if (x <= 0)
						break;

				}
				else if (x <= y) // 약한 전파
				{
					// 전파 대상자는 전파자의 음식까지 신봉
					foodGrid[currentRow][currentCol] |= foodGrid[leaderR][leaderC];

					//전파대상자의 신앙심을 x 만큼 증가
					faithGrid[currentRow][currentCol] += x;

					defense[currentRow][currentCol] = true; //방어 상태 

					//전파자의 간절함은 0. 종료
					x = 0;
					break;

				}
			}
		}


	}

	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << faithGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << foodGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	//결과 출력
	FinalResult();
}

int main(int argc, char** argv)
{
	//int test_case;
	//int T;

	// SWEA 제출 시에는 무조건 주석 처리!
	//freopen("input.txt", "r", stdin);

	//cin >> T;

	//for (test_case = 1; test_case <= T; ++test_case)
	//{

		//cout << "#" << test_case << " "<<endl;
		
		cin >> n >> t; //nxn, t일
		//cout << n <<" "<< t<<endl;

		for (int i = 0; i < n; i++)
		{
			string foodStr="";
			cin >> foodStr;
			for (int j = 0; j < n; j++)
			{
				charFood[i][j] = foodStr[j];
				//cout << foodGrid[i][j] << " ";
			}
			//cout << endl;
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cin >> faithGrid[i][j];
				//cout << faithGrid[i][j]<<" ";
			}
			//cout << endl;
		}

		//숫자로 기본 음식 매핑
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (charFood[i][j] == 'T')
				{
					foodGrid[i][j] = 1;
				}
				else if (charFood[i][j] == 'C')
				{
					foodGrid[i][j] = 2;
				}
				else if (charFood[i][j] == 'M')
				{
					foodGrid[i][j] = 4;
				}
			}
			
		}


		for (int d = 0; d < t; d++)
		{
			//음식조합개수 초기화
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					foodCount[i][j] = 0;
				}
			}
			
			//방어상태 초기화
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					defense[i][j] = false;
				}
			}

			//아침 시간
			morningCompute();

			//점심 시간
			lunchCompute();

			//저녁 시간
			dinnerCompute();
		}
		
	//}

	return 0;
}
