#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<map>

using namespace std;

const int n_max = 20;

int n, m, k;

struct Player {
	int x; //플레이어 위치
	int y;
	int dir; //플레이어 방향
	int score; //플레이어 능력치
	int gun; //총 공격력
	int point; //포인트

};

vector<Player> player;
vector<vector<vector<int>>> guns; //첫번째 vector: 행, 두번째 vector: 열, 세번쨰 vector<int> : 칸에 놓인 총들의 리스트

//상우하좌
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

bool inRange(int x, int y)
{
	return x >= 0 && x < n && y >= 0 && y < n;
}

void isGun(int index, int nx, int ny)
{

	if (!guns[nx][ny].empty()) //총이 있다면
	{
		auto highPos = max_element(guns[nx][ny].begin(), guns[nx][ny].end()); //공격력이 큰 총의 위치
		int highScore = *highPos; //공격력이 큰 총

		//플레이어가 총을 갖고 있는 경우
		if (player[index].gun > 0)
		{
			if (player[index].gun < highScore) //플레이어가 가지고 있는 총의 공격력 < 격자의 총 공격력
			{
				
				int temp = player[index].gun;
				player[index].gun = highScore; //새로운 총 가짐
				*highPos = temp; //교환은 선택된 그 원소를 교체해야 함!!
				//guns[nx][ny].push_back(temp); //격자에 플레이어가 가지고 있던 총 놓음

				//cout << index << " " << player[index].gun << " " << grid[nx][ny] <<endl;
			}
		}
		else //플레이어가 총을 갖고 있지 않은 경우 
		{
			player[index].gun = highScore; //총 공격력 저장
			guns[nx][ny].erase(highPos); //공격력이 가장 큰 총 원소 제거
		}
	}

	//플레이어의 위치 업데이트
	player[index].x = nx;
	player[index].y = ny;

	

}

void losePlayer(int index,int nx, int ny)
{
	//총 내려놓기
	if (player[index].gun > 0)
	{
		//grid[nx][ny] = player[index].gun; 
		guns[nx][ny].push_back(player[index].gun); //**본인이 가진 총 내려놓음
		player[index].gun = 0;
	}

	//다른 플레이어가 있거나 && 격자 밖이면 **빈공간 나올 때 까지 90도 회전

	int dir = player[index].dir;
	for (int i = 0; i < 4; i++)
	{
		int nnx = nx + dr[dir];
		int nny = ny + dc[dir];

		//다른 플레이어가 있는지
		bool player_exist = false;
		for (int i = 0; i < m; i++)
		{
			if (index != i && (nnx == player[i].x && nny == player[i].y))
			{
				player_exist = true;
				break;
			}
		}

		//격자 안이거나, 다른 플레이어가 없다면 이동
		if (inRange(nnx, nny) && !player_exist)
		{
			player[index].x = nnx;
			player[index].y = nny;
			player[index].dir = dir;
			break;
		}

		dir = (dir + 1) % 4; //90도 회전
	}
	

	isGun(index, player[index].x, player[index].y);

}

void isOtherPlayer(int i, int j, int nx, int ny)
{
	
	int me_total = player[i].score + player[i].gun;
	int other_total = player[j].score + player[j].gun;

	int win = j, loser = i;
	if (me_total > other_total)
	{
		win = i;
		loser = j;
	}
	else if (me_total == other_total)
	{
		if (player[i].score > player[j].score)
		{
			win = i;
			loser = j;
		}
	}

	//이기는 경우
	player[win].point += abs(me_total - other_total); //포인트 획득
	
	//진 경우
	losePlayer(loser, nx, ny); //그 다음 이동 칸


	isGun(win, nx, ny); //승자는 격자에 있는 총 vs 가지고 있는 총 -> 가장 공격력 높은 총 획득

	
	
}
void movePlayer()
{

	for (int i = 0; i < m; i++)
	{
		int dir = player[i].dir;//플레이어의 방향
		int nx = player[i].x + dr[dir];
		int ny = player[i].y + dc[dir];
	

		//격자 벗어나면, 정반대 방향으로 바꿔 이동
		if (!inRange(nx, ny)) 
		{
			nx = player[i].x - dr[dir];
			ny = player[i].y - dc[dir];
			player[i].dir = (player[i].dir + 2) % 4; 
			dir = player[i].dir;
		}

		// 일단 이동만 반영
		player[i].x = nx;
		player[i].y = ny;

		// 1) 전투 상대가 있는지 먼저 확인
		int opponent = -1;
		for (int j = 0; j < m; j++) {
			if (i != j && player[j].x == nx && player[j].y == ny) {
				opponent = j;
				break;
			}
		}

		if (opponent != -1) {
			// 2) 전투 먼저
			isOtherPlayer(i, opponent, nx, ny);
			// 전투 내에서 패자 이동/총버리기, 승자 총줍기까지 처리됨
		}
		else {
			// 3) 전투가 없을 때만 총 줍기
			isGun(i, nx, ny);
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
		cin >> n >> m >> k; //격자, 플레이어 수, 라운드 수

		guns.assign(n, vector<vector<int>>(n)); //초기화 //n행. 각행은 n개의 열을 가진 vector<int>
		
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				int val;
				cin >>val;
				if(val>0)
					guns[i][j].push_back(val);
				//cout << grid[i][j] << " ";
			}
			//cout << endl;
		}

		//플레이어 정보 초기화
		player.clear();
		for (int i = 0; i < m; i++)
		{
			int x, y, d, s;
			cin >> x >> y >> d >> s;
			player.push_back({ x-1,y-1,d,s,0,0 });
		}

		for (int i = 0; i < k; i++) //k라운드 동안
		{
			//1. 방향대로 한칸 이동 
			movePlayer();

		}
		
		//출력: 각 플레이어들의 획득한 포인트
		for (int i = 0; i <m; i++)
		{
			cout << player[i].point << " ";
		}
		cout << endl;

	//}
	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}