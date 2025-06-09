#include <iostream>
#include<queue>
#include<algorithm>
using namespace std;

const int R = 80; //5 <= R <= 70
const int C = 80; //5 <= C <= 70
const int K = 1005; //1<= K <=1000 
int row, col, k; //숲의 크기 R행 x C열 & 정령의 수 K
int c, d;
int forest[R+3][C]; //숲
int isExit[R+3][C]; //출구
//상우하좌 - 북(0), 동(1), 남(2), 서(3)
int dr[4] = {-1,0,1,0};
int dc[4] = {0, 1 ,0, -1};
bool visited[R+3][C]; //방문 여부 

int result; //최종 위치 행번호의 합 

bool inRange(int r, int c)
{
    if(3<= r && row+3 > r && 0 <= c && col > c)
        return true;
    else 
        return false;
}

bool canDo(int r, int c)
{
    //AND 연산자 사용해서 숲이 비어있는지 확인 

    // 숲 범위 안에 있는지 체크 
    bool canGo = r+1 < row+3; //아래
    canGo = canGo && 0<= c-1; //왼
    canGo = canGo && c+1 < col; //오
    canGo = canGo && (forest[r-1][c-1]==0); //항상 위에서 아래로 진입하므로, 위에 대각선 왼, 오가 비어있지 않으면 내려올 수 없음 
    canGo = canGo && (forest[r-1][c]==0);
    canGo = canGo && (forest[r-1][c+1]==0);
    canGo = canGo && (forest[r][c-1]==0);
    canGo = canGo && (forest[r][c]==0);
    canGo = canGo && (forest[r][c+1]==0);
    canGo = canGo && (forest[r+1][c]==0);

    return canGo;

}

int bfs(int r, int c)
{
    queue<pair<int,int>> que;

    //방문여부 초기화 
    for(int i=0; i<row+3; i++)
    {
        for(int j=0; j<col; j++)
        {
            visited[i][j]=false;
        }
    }

    int rowMax = r; //행 
    //cout << "rowMax: "<< rowMax <<endl;

    que.push({r,c}); //큐에 푸시하면서 방문처리 
    visited[r][c]=true;

    while(!que.empty()) //큐가 비어있지 않다면 //**while 사용!!
    {
        pair<int,int> twinkle = que.front();
        que.pop();

        for(int i=0; i<4; i++)
        {
            //상하좌우 정령 이동 
            int nextRow = twinkle.first + dr[i];
            int nextCol = twinkle.second + dc[i];

            if(!inRange(nextRow, nextCol)) //범위 안에 없다면 
                continue;

            if(visited[nextRow][nextCol]) //방문했다면
                continue;
            
            //같은 인덱스인 골렘이거나 || **현재 위치가 출구 && 숲 격자가 0이 아닌 경우 
            if(forest[twinkle.first][twinkle.second] == forest[nextRow][nextCol] || (isExit[twinkle.first][twinkle.second]==1 && forest[nextRow][nextCol]!=0))
            {
                que.push({nextRow,nextCol}); 
                visited[nextRow][nextCol]=true;

                rowMax = max(rowMax, nextRow); //현재 행에서 이웃 행으로 이동했을 때, 가장 큰 행 값.

            } 

        }

    }

    return rowMax; // 정령의 최종 위치의 행번호

    

}

void move(int index, int r, int c, int d) //indext, row, col, dir
{

    if(canDo(r+1,c)) //1. 남쪽으로 내려갈 수 있는 경우 
        move(index, r+1, c, d); 
    else if(canDo(r+1, c-1)) //2. 서쪽 방향(반시계)으로 회전 + 아래 = 왼쪽 아래로 이동 
    {
        move(index, r+1, c-1, (d+3)%4); //0(북) -> 3(서)

    }
    else if(canDo(r+1, c+1)) //3. 동쪽 방향(시계)으로 회전 + 아래 = 오른쪽 아래로 이동  
    {
        move(index, r+1, c+1, (d+1)%4); //0(북) -> 1(동)

    }
    else //4. 1,2,3 다 안되는 경우 
    { 
        if(!inRange(r-1, c-1) || !inRange(r-1, c+1)) //숲 범위 안에 있지않을 때 
        {
            //**왼, 오 팔 경로가 범위에서 벗어날 수 있기 때문에 (r-1, c) 보다는 둘 다 고려하는 것이 좋음

            //숲 & 출구 초기화 
            for(int i=0; i<row+3; i++)
            {
                for(int j=0; j<col; j++)
                {
                    forest[i][j]=0;
                    isExit[i][j]=0;
                }
            }

        }else{ //숲 범위 안에 있고, 최대한 남쪽까지 간 경우 

            //골렘 숲 안에 정착 
            forest[r][c] = index; 
            for(int t=0; t<4; t++)
            {
                forest[r+dr[t]][c+dc[t]]=index; //** c+dr[t] 라고 해서 에러 남 
            }

            //골렘의 출구 
            isExit[r+dr[d]][c+dc[d]] = 1;

            //정령의 최종 위치 행번호 저장
            result+= bfs(r,c)-3+1;

        }
    }

}

int main() {

    // 숲의 크기 R행 x C열 & 정령의 수 K 입력
    cin >> row >> col >> k;

    //숲 초기화, 출구 초기화
    for(int i=0; i<row+3; i++)
    {
        for(int j=0; j<col; j++)
        {
            forest[i][j]=0;
            isExit[i][j]=0;
        }
    }

    for(int i=1; i<=k; i++)
    {
        // K 줄 만큼 골렘이 출발하는 열 Ci, 골렘의 출구 방향 정보 di 입력 
        //방향 0~3: 북(0), 동(1), 남(2), 서(3)
        cin >> c >> d;

        move(i, 0, c-1, d); //indext, row, col, dir


    }

    cout << result <<endl; //모든 골렘의 정령의 최종 위치의 행번호의 총합 출력

    return 0;
}