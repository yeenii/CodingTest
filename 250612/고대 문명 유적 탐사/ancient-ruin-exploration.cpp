#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

const int K_MAX = 15;
const int M_MAX = 300;

int M, K;
int historySpace[5][5];
queue<int> que;
vector<pair<int, pair<int, int>>> grid3x3[9];
int centerPoint[9][9];
bool visited[5][5];
int tempGrid[5][5];

int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0,0,-1,1};

struct rotationResults {
	int score; //내림차순
	int angle; //회전 각도
	int col;
	int row;
	int index;
};

vector<rotationResults> rtResult;

vector<pair<int, int>> bfsConnect(int val, int r, int c)
{
	queue < pair<int,int> > connectQue;
	vector<pair<int, int>> group;

	connectQue.push({r,c});
	visited[r][c] = true;

	group.push_back({ r,c });

	while (!connectQue.empty())
	{
		int currentRow = connectQue.front().first;
		int currentCol = connectQue.front().second;
		connectQue.pop();
		

		for (int i = 0; i < 4; i++) //상하좌우 
		{
			int nextRow = currentRow + dr[i];
			int nextCol = currentCol + dc[i];

			if (nextRow < 0 || nextRow >= 5 || nextCol < 0 || nextCol >= 5)
				continue;

			if (visited[nextRow][nextCol])
				continue;

			if (tempGrid[nextRow][nextCol]==val)
			{
				connectQue.push({ nextRow, nextCol });
				visited[nextRow][nextCol] = true;
				group.push_back({ nextRow,nextCol });

			}
		}
	}
	return group;
}

int rotate90(int index, int centerRow, int centerCol, bool isApply) //유물된 조각 수 리턴
{
	//1. 3x3 90도 회전 
	//상대 좌표 공식 (r,c ) -> (c, -r)
	
	//tempGrid 초기화 
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			tempGrid[i][j] = historySpace[i][j];
		}
	}

	for (int i = 0; i < 9; i++)
	{
		int val = grid3x3[index][i].first;

		//(1) 상대 좌표 = 현재 위치 - 중심 좌표 
		int r = grid3x3[index][i].second.first - centerRow;
		int c = grid3x3[index][i].second.second - centerCol;

		//(2) 중심좌표에 (c, -r)
		int dr = centerRow + c;
		int dc = centerCol - r; 

		tempGrid[dr][dc] = val; //2. historySpace와 같은 tempGrid에 저장
	}

	
	//3. BFS로 같은 그룹인지 체크 
	//같은 그룹 3개 이상이면 해당 위치 0으로 
	//tempGrid에 저장
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			visited[i][j] = false;
		}
	}

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << tempGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/


	int fragileNum = 0; //유물된 조각 수 저장
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{

			if (visited[i][j])
				continue;

			//** vector 로 받음
			vector<pair<int,int>> group = bfsConnect(tempGrid[i][j], i, j);

			if (group.size() >= 3) //조각이 3개 이상일 떄 
			{
				fragileNum += group.size();

				for (int k = 0; k < group.size(); k++)
				{
					int r = group[k].first;
					int c = group[k].second;
					tempGrid[r][c] = 0;
					
				}
				
			}
			

		}
	}
	//cout << fragileNum << endl;

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << tempGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	if (isApply)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				historySpace[i][j] = tempGrid[i][j];
			}
		}

	}

	return fragileNum;
	
}

int rotate180(int index, int centerRow, int centerCol, bool isApply) //유물된 조각 수 리턴
{
	//1. 3x3 180도 회전 
	//상대 좌표 공식 (r,c ) -> (c, -r)

	//tempGrid 초기화 
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			tempGrid[i][j] = historySpace[i][j];
		}
	}

	for (int i = 0; i < 9; i++)
	{
		int val = grid3x3[index][i].first;

		//(1) 상대 좌표 = 현재 위치 - 중심 좌표 
		int r = grid3x3[index][i].second.first - centerRow;
		int c = grid3x3[index][i].second.second - centerCol;

		//(2) 중심좌표에 (-r, -c)
		int dr = centerRow -r;
		int dc = centerCol -c;

		tempGrid[dr][dc] = val; //2. historySpace와 같은 tempGrid에 저장
	}

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << tempGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			visited[i][j] = false;
		}
	}

	int fragileNum = 0; //유물된 조각 수 저장
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{

			if (visited[i][j])
				continue;

			vector<pair<int, int>> group = bfsConnect(tempGrid[i][j], i, j);

			if (group.size() >= 3) //조각이 3개 이상일 떄 
			{
				fragileNum += group.size();

				for (int k = 0; k < group.size(); k++)
				{
					int r = group[k].first;
					int c = group[k].second;
					tempGrid[r][c] = 0;

				}

			}


		}
	}
	//cout << fragileNum << endl;

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << tempGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	if (isApply)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				historySpace[i][j] = tempGrid[i][j];
			}
		}

	}

	return fragileNum;

}

int rotate270(int index, int centerRow, int centerCol, bool isApply) //유물된 조각 수 리턴
{
	//1. 3x3 180도 회전 
	//상대 좌표 공식 (r,c ) -> (c, -r)

	//tempGrid 초기화 
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			tempGrid[i][j] = historySpace[i][j];
		}
	}

	for (int i = 0; i < 9; i++)
	{
		int val = grid3x3[index][i].first;

		//(1) 상대 좌표 = 현재 위치 - 중심 좌표 
		int r = grid3x3[index][i].second.first - centerRow;
		int c = grid3x3[index][i].second.second - centerCol;

		//(2) 중심좌표에 (-c, r)
		int dr = centerRow - c;
		int dc = centerCol + r;

		tempGrid[dr][dc] = val; //2. historySpace와 같은 tempGrid에 저장
	}

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << tempGrid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			visited[i][j] = false;
		}
	}

	int fragileNum = 0; //유물된 조각 수 저장
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{

			if (visited[i][j])
				continue;

			vector<pair<int, int>> group = bfsConnect(tempGrid[i][j], i, j);

			if (group.size() >= 3) //조각이 3개 이상일 떄 
			{
				fragileNum += group.size();

				for (int k = 0; k < group.size(); k++)
				{
					int r = group[k].first;
					int c = group[k].second;
					tempGrid[r][c] = 0;

				}

			}


		}
	}
	//cout << fragileNum << endl;


	if (isApply)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				historySpace[i][j] = tempGrid[i][j];
			}
		}

	}

	return fragileNum;

}

void fillZeroSpace()
{

	// 채우는 순서: 열번호가 작은 순 - 행번호 큰 순 
	//유적 벽면에 써있는 숫자는 다시 사용 x. 남은 숫자부터 순서대로 사용 
	vector<pair<int, int>> zeroSpace;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (historySpace[i][j] == 0)
				zeroSpace.push_back({ j,-i }); //열 번호 작은 순 - 행번호 큰 순 으로 저장 
		}
	}

	if (zeroSpace.empty())
		return;

	sort(zeroSpace.begin(), zeroSpace.end());

	for (int i = 0; i < zeroSpace.size(); i++)
	{
		if (que.empty()) //**que.size() < zeroSpace.size() 하지 말기. 작을 때 마다 리턴 당해서 0 채우는 행동을 한번이 아니라 여러번 하게 됨.
			return;

		historySpace[-zeroSpace[i].second][zeroSpace[i].first] = que.front();
		que.pop();
	}

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << historySpace[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/
	
}

int repeatConnectFragile()
{
	//tempGrid 초기화 
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			tempGrid[i][j] = historySpace[i][j];
		}
	}

	//방문 처리
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			visited[i][j] = false;
		}
	}

	//같은 종류 조각 모아 유물로 만들고, 0 처리
	int fragileNum = 0; //유물된 조각 수 저장
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{

			if (visited[i][j])
				continue;

			vector<pair<int, int>> group = bfsConnect(tempGrid[i][j], i, j);

			if (group.size() >= 3) //조각이 3개 이상일 떄 
			{
				fragileNum += group.size();

				for (int k = 0; k < group.size(); k++)
				{
					int r = group[k].first;
					int c = group[k].second;
					tempGrid[r][c] = 0;

				}

			}


		}
	}

	//tempGrid를 historySpace에 옮기기 
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			historySpace[i][j] = tempGrid[i][j];
		}
	}

	//hostorySpace 0 처리된 부분에 유적 벽면에 써있는 숫자 채우기 
	fillZeroSpace();

	/*for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << historySpace[i][j] << " ";
		}
		cout << endl;
	}*/

	return fragileNum;

}

int main(int argc, char** argv)
{
	//int test_case;
	//int T;

	//freopen("input.txt", "r", stdin);
	//cin >> T;

	//for (test_case = 1; test_case <= T; ++test_case)
	//{
		//cout << endl;
		//cout << "#" << test_case << ":"<<endl;

        //5x5 격자 형태 
		//유물 조각 7종류(1-7)
		
		cin >> K >> M; //탐사 반복 횟수 K, 유물 조각 개수 M 
		//cout << K << M<<endl;

		//historySpace 초기화
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				historySpace[i][j] = 0;
				//cout << historySpace[i][j] << " ";
			}
			//cout << endl;
		}

		//유적지에 조각 정보 저장
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				cin >> historySpace[i][j];
				//cout << historySpace[i][j] << " ";
			}
			//cout << endl;
		}

		//M개의 유적의 벽면 숫자를 담는 que를 빈 큐로 초기화. swap()으로 빈큐와 교체
		//** 테스트케이스가 여러개일 경우 '큐'도 초기화 필요
		queue<int> empty;
		que.swap(empty);

		//유적 벽면에 쓰인 숫자
		for (int i = 0; i < M; i++)
		{
			int num = 0;
			cin >> num;
			que.push(num);
			
		}
		
		/*for (int i = 0; i < M; i++)
		{
			 int r = que.front();
			 cout << r << " ";
			 que.pop();
		}
		cout << endl;*/

		//3x3 격자 선택
		for (int i = 1; i <= 3; i++)
		{
			for (int j = 1; j <= 3; j++)
			{
				centerPoint[i][j] = 1;
				//cout << "(" << i << "," << j << ")" << " ";
			}
			//cout << endl;
		}

		//k번 턴

		for (int k = 1; k <= K; k++)
		{
			if (que.empty())
				break;

			//vector 초기화
			for (int i = 0; i < 9; i++)
			{
				grid3x3[i].clear();
			}
			rtResult.clear();

			//[1] 탐사 진행
			// 3x3 격자 내 값 저장
			int count = 0;
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (centerPoint[i][j] == 1)
					{
						//cout << "i: " << i << "j: " << j << endl;
						// 3x3 그리드 내에서
						for (int row = i - 1; row <= i + 1; row++)
						{
							for (int col = j - 1; col <= j + 1; col++)
							{
								int val = historySpace[row][col];
								//cout << val << " ";
								grid3x3[count].push_back({ val, {row, col } });
								
							}
							//cout << endl;
						}
						count++;
						//cout << endl;


					}
				}
			}

			//3x3 격자 회전 - 90도, 180도, 270도
			for (int i = 0; i < 9; i++) //5x5 격자 내 3x3 격자 9개 
			{
				int centerRow = grid3x3[i][4].second.first;
				int centerCol = grid3x3[i][4].second.second;

				int r90 = rotate90(i, centerRow, centerCol, false);
				int r180 = rotate180(i, centerRow, centerCol, false);
				int r270 = rotate270(i, centerRow, centerCol, false);

				//[2] 유물 1차 획득 : 3개 이상 연결된 경우, 조각이 유물이 되어 사라짐 
				//유물 1차 획득 가치 최대화 - 회전한 각도가 가장 작은 방법 - 중심 좌표 열이 가장 작은 구간 - 헹이 가장 작은 구간
				rtResult.push_back({ r90, 90, centerCol, centerRow,i });
				rtResult.push_back({ r180, 180, centerCol, centerRow,i });
				rtResult.push_back({ r270, 270, centerCol, centerRow,i });
			}

			sort(rtResult.begin(), rtResult.end(), [](const rotationResults& a, const rotationResults& b) {
				if (a.score != b.score)return a.score > b.score;
				if (a.angle != b.angle)return a.angle < b.angle;
				if (a.col != b.col)return a.col < b.col;
				if (a.row != b.row)return a.row < b.row;
				return false; //**
				});

			if (rtResult[0].score == 0) //모든 각도 회전 후 유물 획득 결과 0일 경우 종료
				continue;

			rotationResults best = rtResult[0];

			int FinalResult=0;
			if (best.angle == 90)
			{
				FinalResult = rotate90(best.index, best.row, best.col, true);
			}
			else if (best.angle == 180)
			{
				FinalResult = rotate180(best.index, best.row, best.col, true);
			}
			else if (best.angle == 270)
			{
				FinalResult = rotate270(best.index, best.row, best.col, true);
			}

			//cout << best.score << endl; 

			//[3] 유적 벽면에 있는 숫자
			fillZeroSpace();

			//[4] 유물 연쇄 획득 
			//유물 조각이 생겨난 뒤, 조각들 3개 이상 연결될 경우 -> 앞 방식같이 유물이 되어 사라짐 
			// 아직 채울 숫자가 있거나 & 3개 이상 연결되지 않을 때 까지 반복 
			while(true)
			{
				if (que.empty())
					break;

				int result = repeatConnectFragile();
				//cout << result << endl;

				if (result == 0) //**더이상 유물로 만들 수 없는 경우 
					break;

				FinalResult += result;
				
			}

			if (FinalResult == 0) //결과에 0이 나오지 않게 하기 
				break;
			cout << FinalResult << " ";
			
		}
		
		//[5] 탐사 반복
		// [1]~[4] 까지를 1턴. K 번의 턴 진행 
		// 각 턴 마다 유물의 가치의 총합을 출력 
		// 탐사 진행 과정에서 어떤 방법을 사용해도 유물을 획득할 수 없다면, 모든 탐사 즉시 종료 

	//}
	return 0;
}