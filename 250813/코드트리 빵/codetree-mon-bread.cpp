#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<climits>

using namespace std;

const int n_max = 20;
const int m_max = 20;
int n, m;

int grid[n_max][n_max];
int distGrid[n_max][n_max];
vector<pair<int, int>> stPos; //편의점 위치 

int visited[n_max][n_max];
int apply[n_max][n_max];

bool isEnd[m_max];

vector<pair<int,int>> bsPos; //베캠 위치 
vector<pair<int, int>> HPos; //사람 위치 

struct arrBaseCamp {
	int dist;
	int row;
	int col;
};
vector<arrBaseCamp> arrBS; //베캠 정렬 //최단거리 값, 행, 열

int dr[4] = {-1, 0, 0, 1}; //상좌우하
int dc[4] = {0, -1, 1, 0};

bool inRange(int row, int col)
{
	return row >= 0 && row < n && col >= 0 && col < n;
}

void bfs(int t)
{
	queue<pair<int, int>> que;

	//방문처리, 최단거리 저장 배열 초기화
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			visited[i][j] = false;
			distGrid[i][j] = -1;
		}
	}

	int row = stPos[t].first; //t번 편의점
	int col = stPos[t].second;
	que.push({ row, col });
	visited[row][col] = true;
	distGrid[row][col] = 0;

	while (!que.empty())
	{
		int cr = que.front().first;
		int cc = que.front().second;
		que.pop();

		for (int i = 0; i < 4; i++)
		{
			int nr = cr + dr[i];
			int nc = cc + dc[i];

			if (!inRange(nr, nc))
				continue;

			if (visited[nr][nc])
				continue;

			if (apply[nr][nc] )//지나갈 수 없는 베캠 / 편의점
				continue;

			que.push({nr, nc});
			visited[nr][nc] = true;
			distGrid[nr][nc] = distGrid[cr][cc] + 1;
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

		cin >> n >> m;

		//빈공간 0, 베캠 1
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cin >> grid[i][j];
			}
		}

		//편의점 행, 열 입력
		for (int i = 0; i < m; i++)
		{
			int row, col;
			cin >> row >> col;
			stPos.push_back({ row - 1, col - 1 });
			isEnd[i] = false; //도착 여부 초기화
			//cout << stPos[i].first << " " << stPos[i].second << endl;
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				
				apply[i][j] = false;
			}
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (grid[i][j] == 1)
					bsPos.push_back({ i, j });
			}
		}

		//1. 사람 1번 = 편의점 1번
		//편의점 1번 - 베캠까지의 최단거리 구하기
		//베캠 row, col 우선순위별 정렬
		//가장 가까운 베캠에 사람 이동
		int t = 0;
		while(true)
		{
			//격자 위 사람 이동
			for (int c = 0; c < HPos.size(); c++) //격자에 있는 사람 수 만큼
			{
				if (isEnd[c]) //도착한 사람은 건너뛰기
					continue;

				bfs(c);

				/*cout << c << "번째 " << endl;
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						cout << distGrid[i][j] << " ";
					}
					cout << endl;
				}*/

				//사람 위치에서 편의점까지 한칸 이동
				int min_Dist = INT_MAX;
				int minR = -1;
				int minC = -1;
				for (int i = 0; i < 4; i++)
				{
					int nr = HPos[c].first + dr[i];
					int nc = HPos[c].second + dc[i];

					if (!inRange(nr, nc))
						continue;

					if (apply[nr][nc]) continue;

					if (distGrid[nr][nc]< min_Dist) //**주변 셀 4개 중에 가장 작은 셀로 이동
					{
						min_Dist = distGrid[nr][nc];
						minR = nr;
						minC = nc;
						
					}
				}
				HPos[c].first = minR;
				HPos[c].second = minC;
				//cout << c+1 <<": " << HPos[c].first << " " << HPos[c].second << endl;

				//편의점에 도착한 경우
				if (HPos[c].first == stPos[c].first && HPos[c].second == stPos[c].second)
				{
					isEnd[c] = true;
					apply[stPos[c].first][stPos[c].second] = true;
					//cout << c << " 도착" << endl;
				}
					
			}

			

			if (t < m)
			{
				//1. t번째 편의점 - 베캠까지의 최단 거리
				bfs(t);

				/*for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						cout << distGrid[i][j] << " ";
					}
					cout << endl;
				}*/

				//2. t번 편의점의 `베캠 우선순위 별 정렬`
				for (int i = 0; i < bsPos.size(); i++)
				{
					int bsR = bsPos[i].first;
					int bsC = bsPos[i].second;

					if (apply[bsR][bsC])
						continue;

					arrBS.push_back({ distGrid[bsR][bsC],bsR,bsC }); //최단 거리 값, 행, 열
				}

				sort(arrBS.begin(), arrBS.end(), [](const arrBaseCamp& a, const arrBaseCamp& b) {
					if (a.dist != b.dist) return a.dist < b.dist;
					if (a.row != b.row) return a.row < b.row;
					if (a.col != b.col) return a.col < b.col;
					return false;
					}); //모두 오름차순


				//3. 가장 가까운 베캠에 사람 이동
				HPos.push_back({arrBS[0].row ,arrBS[0].col});

				apply[arrBS[0].row][arrBS[0].col] = true; //해당 베캠 못 지나감
				
				arrBS.clear();
			}
		
			
			//편의점에 모든 사람이 도착했는지 체크
			int cnt = 0;
			for (int i = 0; i < m; i++)
			{
				if (isEnd[i])
					cnt++;
			}

			if (cnt == m) //도착한 사람 수가 사람 수와 일치한다면 break
			{
				cout << t + 1 << endl; //결과 출력 
				break;
			}

			t++;
		}
	//}



	
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}