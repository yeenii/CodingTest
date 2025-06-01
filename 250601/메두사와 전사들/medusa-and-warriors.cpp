#include <iostream>
#include<algorithm> //max() 사용 
#include <queue>
using namespace std;

const int N = 60; //4 <= N <= 50
const int M = 310; // 0<= M <= 300
const int INF = 1e9+10; //무제한 큰 상수 

int villageSize =0;
int warriorSize =0;
int medusaHomeRow, medusaHomeCol =0; //메두사 집 위치 
int parkRow, parkCol=0; //공원 위치

int road[N][N]; //도로 O: 0, X: -1 구분 그리드 
int distanceGrid[N][N]; //거리 저장. 기본은 -1, 장애물은 INF 처리   

pair<int, int> warriorsPosition[M]; //전사들의 위치.  {-1, -1}은 사라진 전사. 
int warriorGrid[N][N]; //전사들의 그리드 상 위치

//상하좌우
int drow[4] = {-1,1,0,0}; //상하
int dcol[4] = {0,0,-1,1}; //좌우

//방문 여부
//bool visited[N][N];

void bfsShortRoad(int parkRow, int parkCol)
{

    //큐에 푸시 - 방문 처리
    queue<pair<int,int>> que;

    que.push({parkRow,parkCol}); //시작 지점 큐에 추가 
    //visited[parkRow][parkCol]=true; //방문 처리 
    road[parkRow][parkCol]=0; //시작지점은 0처리 

    //큐가 비어있지 않다면
    while(!que.empty())
    {
        int currentRow = que.front().first;
        int currentCol = que.front().second;

        //큐에서 pop하고 
        que.pop();

        for(int i=0; i<4; i++)
        {
            int nextRow = currentRow + drow[i];
            int nextCol = currentCol + dcol[i];

            //격자 밖을 벗어나는지 
            if(nextRow<0 || nextRow >=villageSize || nextCol <0 || nextCol >= villageSize)
                continue;

            //방문했는지 
            //if(visited[nextRow][nextCol])
                //continue;

            //도로가 아닌 경우 continue
            if(road[nextRow][nextCol]!= -1) 
                continue;
            
            //다음 셀의 거리 업데이트 
            road[nextRow][nextCol] = road[currentRow][currentCol]+1;
            que.push({nextRow, nextCol}); //팝한거와 인접한 노드 비교해서 큐에 푸시

        }
    }  
    

}

int sightMap[N][N]; // 시야 모양

int sightUp(int x, int y, bool isTest)
{
    int warriorsCount =0;
    int tempSight[N][N];

    //tempSight 초기화
    for(int i=0; i<villageSize; i++)
    {
        for(int j=0; j< villageSize; j++)
        {
            tempSight[i][j]=0;
        }
    }

    //1. 기본 시야 모양 생성
    for(int i=x-1; i>=0; i--) //그리드 내 윗방향으로 
    {
        int left = max(0, y - (x-i));
        int right = min(villageSize-1, y + (x-i));

        for(int j=left; j<=right; j++)
        {
            tempSight[i][j]=1; //시야 처리 
        }

    }

    //2. 전사에 의한 잘림 여부 판단
    //2-1. 중앙 직선
    bool obstruction = false; //전사가 있는지 여부 
    for(int i=x-1; i>=0; i--)
    {
        if(obstruction==true) //전사가 있으면, 그 뒤에 셀들은 시야 제거 
            tempSight[i][y]=0;
        else
            tempSight[i][y]=1; //전사가 없으면, 시야 유지 

        if(warriorGrid[i][y]) //한칸 위 중앙에 전사가 있으면
            obstruction = true; 

    }

    //2-1. 좌, 우 대각선
    for(int i=x-1; i>=1; i--) //왼쪽 
    {
        int left = max(0, y - (x-i));
        int right = min(villageSize-1, y + (x-i));

        for(int j=left; j<y; j++)
        {
            if(tempSight[i][j]==0 || warriorGrid[i][j]) //만약에, 한칸 위 좌측에 전사가 있으면 
            {
                if(j>0) //왼쪽 측면 조정 
                    tempSight[i-1][j-1] = 0; //두칸 위 대각선 셀 시야 제거 
                tempSight[i-1][j]=0; 
            }

        }

        for(int j= y+1; j<=right; j++)
        {
            if(tempSight[i][j]==0 || warriorGrid[i][j]) //만약에, 한칸 위 우측에 전사가 있으면 
            {
                if(j+1 < villageSize) //오른쪽 측면 조정
                    tempSight[i-1][j+1] =0; //두칸 위 대각선 셀 시야 제거 
                tempSight[i-1][j] =0;

            }
        }

        

    }

    for(int i=0; i<villageSize; i++)
    {
        for(int j=0; j< villageSize; j++)
        {
            //cout << warriorGrid[i][j] << " ";
        }
        //cout <<endl;
    }
    //3. 시야 내 전사 수 계산 
    for(int i=x-1; i>=0; i--) //그리드 내 윗방향으로 
    {
        int left = max(0, y - (x-i));
        int right = min(N-1, y + (x-i));

        for(int j=left; j<=right; j++)
        {
            if(tempSight[i][j]==1) //전사 뒤 셀 제거한 시야
                warriorsCount += warriorGrid[i][j]; //시야 내에 있는 모든 전사 수를 더함 
        }

    }


    if(!isTest) //테스트가 아닌 적용하는 경우 (true: test, false: 적용)
    {
        for(int i=0; i<villageSize; i++)
        {
            for(int j=0; j< villageSize; j++)
            {
                sightMap[i][j]= tempSight[i][j];
            }
        }
    }

    return warriorsCount;
}

int sightDown(int x, int y, bool isTest)
{
    int warriorsCount =0;
     int tempSight[N][N];

    //sightMap 초기화
    for(int i=0; i<villageSize; i++)
    {
        for(int j=0; j< villageSize; j++)
        {
            tempSight[i][j]=0;
        }
    }

    //1. 기본 시야 모양 생성
    for(int i=x+1; i<villageSize; i++) //그리드 내 윗방향으로 
    {
        int left = max(0, y - (i-x));
        int right = min(villageSize-1, y + (i-x));

        for(int j=left; j<=right; j++)
        {
            tempSight[i][j]=1; //시야 처리 
        }

    }

    //2. 전사에 의한 잘림 여부 판단
    //2-1. 중앙 직선
    bool obstruction = false; //전사가 있는지 여부 
    for(int i=x+1; i<villageSize; i++)
    {
        if(obstruction==true) //전사가 있으면, 그 뒤에 셀들은 시야 제거 
            tempSight[i][y]=0;
        else
            tempSight[i][y]=1; //전사가 없으면, 시야 유지 

        if(warriorGrid[i][y]) //한칸 위 중앙에 전사가 있으면
            obstruction = true; 

    }

    //2-1. 좌, 우 대각선
    for(int i=x+1; i<villageSize-1; i++) //왼쪽 
    {
        int left = max(0, y - (i-x));
        int right = min(villageSize-1, y + (i-x));

        for(int j=left; j<y; j++)
        {
            if(tempSight[i][j]==0 || warriorGrid[i][j]) //만약에, 한칸 아래 좌측에 전사가 있으면 
            {
                if(j>0) //왼쪽 측면 조정 
                    tempSight[i+1][j-1] = 0; //두칸 아래 대각선 셀 시야 제거 
                tempSight[i+1][j]=0; 
            }

        }

        for(int j= y+1; j<=right; j++)
        {
            if(tempSight[i][j]==0 || warriorGrid[i][j]) //만약에, 한칸 아래 우측에 전사가 있으면 
            {
                if(j+1 < villageSize) //오른쪽 측면 조정
                    tempSight[i+1][j+1] =0; //두칸 아래 대각선 셀 시야 제거 
                tempSight[i+1][j] =0;

            }
        }

        

    }


    //3. 시야 내 전사 수 계산 
    for(int i=x+1; i<villageSize; i++) //그리드 내 아래 방향으로 
    {
        int left = max(0, y - (i-x));
        int right = min(villageSize-1, y + (i-x));

        for(int j=left; j<=right; j++)
        {
            if(tempSight[i][j]==1) //전사 뒤 셀 제거한 시야
                warriorsCount += warriorGrid[i][j]; //시야 내에 있는 모든 전사 수를 더함 
        }

    }


    if(!isTest) //테스트가 아닌 적용하는 경우 (true: test, false: 적용)
    {
        for(int i=0; i<villageSize; i++)
        {
            for(int j=0; j< villageSize; j++)
            {
                sightMap[i][j]= tempSight[i][j];
            }
        }
    }


    return warriorsCount;

}

int sightLeft(int x, int y, bool isTest)
{
    int warriorsCount =0;
     int tempSight[N][N];

    //sightMap 초기화
    for(int i=0; i<villageSize; i++)
    {
        for(int j=0; j< villageSize; j++)
        {
            tempSight[i][j]=0;
        }
    }

    //1. 기본 시야 모양 생성
    for(int i=y-1; i>=0; i--) //그리드 내 왼쪽방향으로 
    {
        int top = max(0, x - (y-i));
        int bottom = min(villageSize-1, x + (y-i));

        for(int j=top; j<=bottom; j++)
        {
            tempSight[j][i]=1; //시야 처리 
        }

    }

    //2. 전사에 의한 잘림 여부 판단
    //2-1. 중앙 직선
    bool obstruction = false; //전사가 있는지 여부 
    for(int i=y-1; i>=0; i--)
    {
        if(obstruction==true) //전사가 있으면, 그 뒤에 셀들은 시야 제거 
            tempSight[x][i]=0;
        else
            tempSight[x][i]=1; //전사가 없으면, 시야 유지 

        if(warriorGrid[x][i]) //한칸 위 중앙에 전사가 있으면
            obstruction = true; 

    }

    //2-1. 좌, 우 대각선
    for(int i=y-1; i>0; i--) //왼쪽 
    {
        int top = max(0, x - (y-i));
        int bottom = min(villageSize-1, x + (y-i));

        for(int j=top; j<x; j++)
        {
            if(tempSight[j][i]==0 || warriorGrid[j][i]) //만약에, 한칸 아래 좌측에 전사가 있으면 
            {
                if(j>0) //왼쪽 측면 조정 
                    tempSight[j-1][i-1] = 0; //두칸 아래 대각선 셀 시야 제거 
                tempSight[j][i-1]=0; 
            }

        }


        for(int j= x+1; j<=bottom; j++)
        {
            if(tempSight[j][i]==0 || warriorGrid[j][i]) //만약에, 한칸 아래 우측에 전사가 있으면 
            {
                if(j+1 < villageSize) //오른쪽 측면 조정
                    tempSight[j+1][i-1] =0; //두칸 아래 대각선 셀 시야 제거 
                tempSight[j][i-1] =0;

            }
        }

        

    }


    //3. 시야 내 전사 수 계산 
    for(int i=y-1; i>=0; i--) //그리드 내 아래 방향으로 
    {
        int top = max(0, x - (y-i));
        int bottom = min(villageSize-1, x + (y-i));

        for(int j=top; j<=bottom; j++)
        {
            if(tempSight[j][i]==1) //전사 뒤 셀 제거한 시야
                warriorsCount += warriorGrid[j][i]; //시야 내에 있는 모든 전사 수를 더함 
        }

    }

    

     if(!isTest) //테스트가 아닌 적용하는 경우 (true: test, false: 적용)
    {
        for(int i=0; i<villageSize; i++)
        {
            for(int j=0; j< villageSize; j++)
            {
                sightMap[i][j]= tempSight[i][j];
            }
        }
    }

    return warriorsCount;

}

int sightRight(int x, int y, bool isTest)
{
    int warriorsCount =0;
     int tempSight[N][N];

    //sightMap 초기화
    for(int i=0; i<villageSize; i++)
    {
        for(int j=0; j< villageSize; j++)
        {
            tempSight[i][j]=0;
        }
    }

    //1. 기본 시야 모양 생성
    for(int i=y+1; i<villageSize; i++) //그리드 내 왼쪽방향으로 
    {
        int top = max(0, x - (i-y));
        int bottom = min(villageSize-1, x + (i-y));

        for(int j=top; j<=bottom; j++)
        {
            tempSight[j][i]=1; //시야 처리 
        }

    }

    //2. 전사에 의한 잘림 여부 판단
    //2-1. 중앙 직선
    bool obstruction = false; //전사가 있는지 여부 
    for(int i=y+1; i<villageSize; i++)
    {
        if(obstruction==true) //전사가 있으면, 그 뒤에 셀들은 시야 제거 
            tempSight[x][i]=0;
        else
            tempSight[x][i]=1; //전사가 없으면, 시야 유지 

        if(warriorGrid[x][i]) //한칸 위 중앙에 전사가 있으면
            obstruction = true; 

    }

    //2-1. 좌, 우 대각선
    for(int i=y+1; i<villageSize-1; i++) //왼쪽 
    {
        int top = max(0, x - (i-y));
        int bottom = min(villageSize-1, x + (i-y));

        for(int j=top; j<x; j++)
        {
            if(tempSight[j][i]==0 || warriorGrid[j][i]) //만약에, 한칸 아래 좌측에 전사가 있으면 
            {
                if(j>0) //왼쪽 측면 조정 
                    tempSight[j-1][i+1] = 0; //두칸 아래 대각선 셀 시야 제거 
                tempSight[j][i+1]=0; 
            }

        }

        for(int j= x+1; j<=bottom; j++)
        {
            if(tempSight[j][i]==0 || warriorGrid[j][i]) //만약에, 한칸 아래 우측에 전사가 있으면 
            {
                if(j+1 < villageSize) //오른쪽 측면 조정
                    tempSight[j+1][i+1] =0; //두칸 아래 대각선 셀 시야 제거 
                tempSight[j][i+1] =0;

            }
        }

        
        

    }

    //3. 시야 내 전사 수 계산 
    for(int i=y+1; i<villageSize; i++) //그리드 내 아래 방향으로 
    {
        int top = max(0, x - (i-y));
        int bottom = min(villageSize-1, x + (i-y));
        for(int j=top; j<=bottom; j++)
        {
            if(tempSight[j][i]==1) //전사 뒤 셀 제거한 시야
                warriorsCount += warriorGrid[j][i]; //시야 내에 있는 모든 전사 수를 더함 
        }

    }

     if(!isTest) //테스트가 아닌 적용하는 경우 (true: test, false: 적용)
    {
        for(int i=0; i<villageSize; i++)
        {
            for(int j=0; j< villageSize; j++)
            {
                sightMap[i][j]= tempSight[i][j];
            }
        }
    }

    return warriorsCount;

}

int chooseBestSight(int currentRow, int currentCol) //현재 row, 현재 col, 현재 시야 적용 여부 
{

    for(int i=0; i<villageSize; i++)
    {
        for(int j=0; j<villageSize; j++)
        {
            sightMap[i][j]=0;
        }
    }

    int maxCount = -1;
    int dir = -1;

    int countSight[4];

    //전사 수가 많은 시야를 sightMap에 저장하기 위해 isTest 사용
    //isTest - true : 테스트용. 0으로 초기화 | false: 적용 
    countSight[0] = sightUp(currentRow, currentCol, true); 
    countSight[1] = sightDown(currentRow, currentCol, true);
    countSight[2] = sightLeft(currentRow, currentCol, true);
    countSight[3] = sightRight(currentRow, currentCol, true);

    //가장 많은 `돌이된 전사`를 가지고 있는 시야 방향을 구함 
    for(int i=0; i<4; i++)
    {
        if(maxCount < countSight[i])
        {
            maxCount = countSight[i];
            dir = i;
        }
    }

    //**전사 수가 동일한 시야가 여러개일 경우, 상하좌우 우선순위로 해야하기 때문에 else if 써야 됨.
    if(dir == 0)
        return sightUp(currentRow, currentCol, false);      
    else if(dir == 1)
        return sightDown(currentRow, currentCol, false);
    else if(dir ==2)
        return sightLeft(currentRow, currentCol, false);
    else if(dir == 3)
        return sightRight(currentRow, currentCol, false);
    
    
}

int calculateManhattanDistance(int currentRow, int currentCol, int warriorRow, int warriorCol)
{
    return (abs(currentRow-warriorRow) + abs(currentCol-warriorCol));
}

pair<int, int> warriorMoveAndAttack(int currentRow, int currentCol)
{
    int warriorMoveCount =0; //전사 이동한 거리 

    //전사 수 
    for(int i=0; i<warriorSize; i++)
    {
        if(warriorsPosition[i].first == -1 && warriorsPosition[i].second == -1) //이전에 잡혔던 전사는 넘어가기 
            continue;
            
        int warriorRow = warriorsPosition[i].first;
        int warriorCol = warriorsPosition[i].second;

        //돌이된 전사는 넘어가기 
        if(sightMap[warriorRow][warriorCol])
            continue;
            
        //현재 메두사의 위치와 전사의 거리를 맨해튼 거리 계산으로 비교 
        int distance = calculateManhattanDistance(currentRow, currentCol, warriorRow, warriorCol);
        bool canMove = false;

        //1. 첫번째 이동 : 상하좌우 순  
        for(int dir=0; dir<4; dir++)
        {
            //이웃 셀로 이동 
            int neighRow = warriorRow + drow[dir];
            int neighCol = warriorCol + dcol[dir];

            //격자 밖일 경우 건너뛰기 
            if(neighRow <0 || neighRow >= villageSize || neighCol <0 || neighCol >= villageSize)
                continue;

            //** 시야 내에 있는 경우 건너뛰기 
            if(sightMap[neighRow][neighCol]==1)
                continue;
                
            int newDistance = calculateManhattanDistance(currentRow, currentCol, neighRow, neighCol);
            if(newDistance < distance)
            {
                warriorRow = neighRow;
                warriorCol = neighCol;
                warriorMoveCount++;
                canMove = true;
                break; //한번 칸 이동하면 break;
            }

        }
    

        //2. 두번째 이동
        int twiceDistance = calculateManhattanDistance(currentRow, currentCol, warriorRow, warriorCol);
        //좌우 상하 설정 
        int anotherDrow[4] = {0,0,-1,1}; 
        int anotherDcol[4] = {-1, 1, 0, 0};

        if(canMove)
        {
            for(int dir=0; dir<4; dir++)
            {
                int neighRow = warriorRow + anotherDrow[dir];
                int neighCol = warriorCol + anotherDcol[dir];

                //격자 밖일 경우 건너뛰기 
                if(neighRow <0 || neighRow >= villageSize || neighCol <0 || neighCol >= villageSize)
                    continue;

                //** 시야 내에 있는 경우 건너뛰기 
                if(sightMap[neighRow][neighCol]==1)
                    continue;
                
                int newDistance = calculateManhattanDistance(currentRow, currentCol, neighRow, neighCol);
                if(newDistance < twiceDistance)
                {
                    warriorRow = neighRow;
                    warriorCol = neighCol;
                    warriorMoveCount++;
                    break; //한번 칸 이동하면 break;
                }
            }
        }
        

        //전사 위치 업데이트 
        warriorsPosition[i].first = warriorRow;
        warriorsPosition[i].second = warriorCol;

    }

    int catchWarriorCount =0; //잡힌 전사 수 
    //메두사와 같은 위치에 있는 전사는 잡힌 상태로 표시 
    for(int i=0; i<warriorSize; i++)
    {
        if(warriorsPosition[i].first == -1 && warriorsPosition[i].second == -1) //이전에 잡혔던 전사는 넘어가기 
            continue;
            
        //메두사와 같은 위치
        if(warriorsPosition[i].first == currentRow && warriorsPosition[i].second == currentCol)
        {
            //전사 잡힌 상태로 표시 
            catchWarriorCount++;
            warriorsPosition[i].first = -1;
            warriorsPosition[i].second = -1; 
        }
    }

    //for(int i=0; i<warriorSize; i++)
    //{
        //cout << "x:"<<warriorsPosition[i].first <<" y:" << warriorsPosition[i].second <<endl;
    //}

    //cout << "x: " << currentRow << "y" << currentCol <<endl;//(4,1) 어디감..?

    return {warriorMoveCount, catchWarriorCount};

    

}


int main() {
    
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    //마을 크기 N, 전사의 수 M이 공백을 사이에 두고 입력 
    //정수로 입력되니까 공백 고려X
    cin >> villageSize >> warriorSize;

    //메두사의 집의 위치 정보 (s_r, s_c),  공원의 위치 정보 (e_r, e_c) 입력 
    cin >> medusaHomeRow >> medusaHomeCol >> parkRow >> parkCol;

    //M명의 전사들의 좌표 (a1r, a1c), (a2r, a2c), (a3r, a3c), (a4r, a4c) 입력
    for(int i=0; i<warriorSize; i++)
    {
        cin >> warriorsPosition[i].first >> warriorsPosition[i].second;
        //cout << warriorsPosition[i].first << " " << warriorsPosition[i].second << endl;
    }
    
    //N 줄에 걸쳐 마을의 도로 정보 입력. 도로 O = 0, 도로 X = 1
    for(int i=0; i< villageSize; i++)
    {
        for(int j=0; j<villageSize; j++)
        {
            cin >> road[i][j];
        }

    }

    //도로가 아닌 경우 -1 처리 
    for(int i=0; i< villageSize; i++)
    {
        for(int j=0; j<villageSize; j++)
        {
            if(road[i][j]==1)
                road[i][j] = INF; //도로가 아닌 경우는 INF
            
            if(road[i][j]==0)
                road[i][j] = -1; //도로인 경우 -1 
            //cout << road[i][j] <<" ";
        }

        //cout << endl;
    }


    //집에서 공원까지 최단 경로 구하기 
    bfsShortRoad(parkRow, parkCol);
    
    if(road[medusaHomeRow][medusaHomeCol]==-1) //**메두사의 집 - 공원까지 이어지는 도로가 존재하지 않는 경우 = 최단 경로 구한 이후에도, 여전히 -1인 경우
    {
        cout << "-1\n"<<endl;
        return 0;
    }

    //최단 경로 출력
    //for(int i=0; i<villageSize; i++)
    //{
        //for(int j=0; j<villageSize; j++)
        //{
            //cout << road[i][j] << " ";
        //}
        //cout <<endl;
    //}

    
    int currentRow = medusaHomeRow;
    int currentCol = medusaHomeCol;

    while(true) //메두사가 공원에 도달할 때까지 반복
    {
        // 1. 메두사 이동 
        for(int i=0; i<4; i++)
        {
            //상하좌우 순으로 
            int nextRow = currentRow + drow[i];
            int nextCol = currentCol + dcol[i];

            //격자 밖으로 벗어나는 경우 건너뛰기 
            if(nextRow <0 || nextRow >= villageSize || nextCol < 0 || nextCol >= villageSize )
                continue;

            //최단 경로로 현재 위치 보다 작은 위치로 메두사 이동 
            if(road[nextRow][nextCol] < road[currentRow][currentCol])
            {
                currentRow = nextRow;
                currentCol = nextCol;
                break;
            }
        }

        //cout << "x: " << currentRow << " y:" << currentCol <<endl;
        
        //메두사가 공원에 도착했을 때 - **메두사가 한칸만 가도 공원일 수 있으니 
        if(road[currentRow][currentCol] == road[parkRow][parkCol])
        {
            cout << "0\n" <<endl;
            break;

        }  
        
        for(int i=0; i< warriorSize; i++)
        {
            //메두사가 이동한 위치에 전사가 있는 경우 
            if(warriorsPosition[i].first == currentRow && warriorsPosition[i].second == currentCol)
            {
                
                //전사 잡힘 
                warriorsPosition[i].first = -1; 
                warriorsPosition[i].second = -1; 
            }   

        }
        
        //2. 메두사 시선 
        for(int i=0; i<villageSize; i++)
        {
            for(int j=0; j<villageSize; j++)
            {
                warriorGrid[i][j] =0;
            }
        }

        for(int i=0; i<warriorSize; i++)
        {
            if(warriorsPosition[i].first == -1 && warriorsPosition[i].second == -1)
                continue;
            
            int x = warriorsPosition[i].first;
            int y = warriorsPosition[i].second;

            warriorGrid[x][y]++; //**전사는 한 셀에 여러명있을 수 있음 

        }

        //이 중 최대 전사 수가 나오는 시야
        int maxWarriorStone = chooseBestSight(currentRow, currentCol);

        //3. 전사들의 이동 
        auto result = warriorMoveAndAttack(currentRow, currentCol); 

        int warriorMove = result.first;
        int warriorAttack = result.second;

        cout << warriorMove << " " << maxWarriorStone << " " << warriorAttack <<endl;
        

    }


    return 0;
}