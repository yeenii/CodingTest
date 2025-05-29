#include <iostream>
#include<vector>
#include<string> //string 사용 
#include<algorithm>

using namespace std;

const int N = 51;
const int T = 31;

int foodGrid, dayCount=0; //4, 2

//상하좌우
int drow[4] = {-1, 1, 0, 0};
int dcol[4] = {0, 0, -1, 1};

int foodArray[N][N]; //신봉 음식 배열
int faithArray[N][N]; //신앙심 배열 

bool visited[N][N]; //방문 상태
bool defended[N][N]; //**방어 상태 

int currentGroupSize;
pair<int, int> leadersPoint[N]; //각 그룹내 대표의 좌표

vector<pair<int, pair<int, int>>> groupMembers; //**그룹 내 신앙심 순 정렬 = {신앙심, (row, col)};
vector<pair<pair<int,int>, pair<int,int>>> leadersList; //대표들의 신앙심, 신봉 음식, 좌표 저장 

void morningTime()
{
    for(int i=0; i<foodGrid; i++)
    {
        for(int j=0; j<foodGrid; j++)
        {
            faithArray[i][j]++;

            //cout << faithArray[i][j] << " ";
        }
        //cout <<endl;
    }


}

void dfsConnectFood(int row, int col, char foodId)
{
    //방문 처리
    visited[row][col] = true;

    //{신앙심, {row, col}}
    groupMembers.push_back({-faithArray[row][col], {row, col}}); //pair는 무조건 {}

    //현재 그룹의 크기 
    currentGroupSize++;

    for(int i=0; i<4; i++)
    {
        int neighRow = row + drow[i];
        int neighCol = col + dcol[i];

        //영역 넘어가지 않는지
        if(neighRow < 0 || neighRow >= foodGrid || neighCol < 0 || neighCol >=foodGrid)
            continue;

        //방문 처리 안됐는지 
        if(visited[neighRow][neighCol])
            continue;

        //다른 신봉 음식인지
        if(foodId != foodArray[neighRow][neighCol])
            continue;  

        dfsConnectFood(neighRow, neighCol, foodId);
    }

}

int countBasicFoods(int food)
{
    int count =0;

    //**비트마스크로 기본 음식 민트(1), 초코(2), 우유(4) 여부 확인
    //if문으로 각각 따로 해야지 기본 음식 말고도 민트초코, 민트우유, 초코우유, 민트초코우유 조합이 가능함. 

    if(food & 1)
        count ++;
    
    if (food & 2)
        count ++;

    if(food & 4)
        count++;

    return count;

}

void lunchTime()
{
    leadersList.clear(); //**대표 리스트 초기화 -> 둘째날 점심에 다시 사용해야 함

    //방문 여부 초기화
    for(int row=0; row<foodGrid; row++)
    {
        for(int col=0; col<foodGrid; col++)
        {
            visited[row][col] = false;
        }
    }

    //인접한 학생들과 신봉음식이 완전히 같은 경우 
    for(int row=0; row<foodGrid; row++)
    {
        for(int col=0; col<foodGrid; col++)
        {
            
            //이미 방문한 경우
            if(visited[row][col])
                continue;
            
            groupMembers.clear(); //**초기화
            currentGroupSize=0;
            
            int currentFoodId = foodArray[row][col];
            //cout << "currentFoodId: "<<currentFoodId <<endl;

            //1. dfs로 같은 그룹 구함 
            dfsConnectFood(row, col, currentFoodId);

            //2. 신앙심 순 정렬 
            sort(groupMembers.begin(), groupMembers.end());

            //3. 대표 좌표 구하기 
            int leaderRow = groupMembers[0].second.first; //제일 신앙심이 큰 학생의 row
            int leaderCol = groupMembers[0].second.second; //제일 신앙심이 큰 학생의 col
            //cout << "leaderRow: " << leaderRow <<endl;
            //cout << "leaderCol: " << leaderCol <<endl;

            //**4. 대표에게 그룹크기-1 만큼 신앙심 넘기고, 그룹원은 신앙심-1 하기 
            faithArray[leaderRow][leaderCol] += currentGroupSize -1; 
            for(int i=1; i<groupMembers.size();i++)
            {
                int dr = groupMembers[i].second.first;
                int dc = groupMembers[i].second.second; 

                faithArray[dr][dc]-=1; //대표 포함 모든 그룹원에 신앙심 -1 하기 
            }

            int basicFoodCount = countBasicFoods(foodArray[row][col]); //**현재 셀의 기본 음식 개수 구하기 

            //**5. 대표 리스트 저장 : 기본 음식 개수 , 신앙심, 좌표 - 기본 음식이 가장 작은 순에서 신앙심이 큰 대표 순으로 정렬됨 
            leadersList.push_back({{basicFoodCount, -faithArray[leaderRow][leaderCol]}, {leaderRow,leaderCol}});
            
            
        }            

        
    }
    sort(leadersList.begin(), leadersList.end()); //대표 리스트 정렬 

    
    //cout << leadersList[0].first.second<<endl;

    //cout << "---faithArray--" <<endl;
    //for(int row=0; row<foodGrid; row++)
    //{
        //for(int col=0; col<foodGrid; col++)
        //{
            //cout << faithArray[row][col] <<" ";
        //}   
        //cout <<endl;
    //}
    
    
}


void dinnerTime()
{

    //방어 상태 초기화 
    for(int row=0; row<foodGrid; row++)
    {
        for(int col=0; col <foodGrid; col++)
        {
            defended[row][col] = false;
        }
    }

    //대표 = 전파자 
    //전파자의 신앙심에서 1만 남기고, 간절함 x를 B-1로 바꿔 전파에 사용 
    for(auto leader: leadersList) //신앙심이 가장 큰 대표부터 
    {
        int leaderRow = leader.second.first; //대표의 row
        int leaderCol = leader.second.second; //대표의 col
        //cout << "leaderRow: "<< leaderRow << "leaderCol: "<< leaderCol <<endl;

        //대표가 방어상태일 경우 다음 대표로 건너뛰기 
        if(defended[leaderRow][leaderCol])
            continue; 

        //간절함 
        int x = faithArray[leaderRow][leaderCol]-1;
        //cout << "x: "<< x <<endl;

        //전파 방향 - 나머지 0(위), 1(아래), 2(왼) ,3(오)
        int dir = faithArray[leaderRow][leaderCol] % 4; 
        //cout << "dir: "<< dir <<endl;

        //대표 신앙심 1로 수정  
        faithArray[leaderRow][leaderCol] = 1; 

        //전파 대상자 좌표
        int currentRow = leaderRow; //**
        int currentCol = leaderCol;

        while(true)
        {
            //**대표 좌표서 부터 dir 방향으로 1칸씩 이동 
            currentRow += drow[dir];
            currentCol += dcol[dir];

            //조건 1: 격자 밖인 경우 전파 종료 
            if(currentRow < 0 || currentRow >= foodGrid || currentCol <0 || currentCol >= foodGrid)
                break;
            
            //조건 2: 간절함 x = 0 인 경우 전파 종료 
            if(x<=0)
                break;
            
            //전파자와 전파 대상자의 신봉 음식이 같은 경우 건너 뜀. 
            if(foodArray[currentRow][currentCol] == foodArray[leaderRow][leaderCol])
            {
                continue;
            }

            //대표의 간절함 x > 전파 대상자의 신앙심 y
            int y = faithArray[currentRow][currentCol]; //전파 대상자의 신앙심 
            //cout << "y: "<< y <<endl;
            //cout << x <<endl;

            if(x > y) //강한 전파 
            {
                //전파자와 동일한 음식으로 변경
                foodArray[currentRow][currentCol] = foodArray[leaderRow][leaderCol]; 
                //cout << foodArray[currentRow][currentCol] <<endl;

                //전파자 간절함 x는 y+1 만큼 깎임 
                x-= (y+1);
                //cout <<x <<endl;

                //전파 대상자의 신앙심은 +1 
                faithArray[currentRow][currentCol]++;
                //cout << faithArray[currentRow][currentCol] << endl;

                defended[currentRow][currentCol]=true; //방어상태로 만들기  

                //이때, x=0이되면  전파 종료 
                if(x<=0)
                    break;

            }else if(x<= y) //약한 전파 
            {
                //전파자의 음식 + 전파 대상자의 음식 모두 신봉 
                foodArray[currentRow][currentCol] |= foodArray[leaderRow][leaderCol]; //전파 대상자에게 전파자의 음식 더함(|)
                //controlFood(currentRow, currentCol, leaderRow, leaderCol);

                //전파 대상자의 신앙심은 x만큼 증가
                faithArray[currentRow][currentCol] +=x;
                //cout << faithArray[currentRow][currentCol] <<endl; 

                defended[currentRow][currentCol]=true; //방어상태로 만들기 

                //전파자 간절함 x는 0되고, 전파 안함 
                x=0;                    
                break;

            }

        }

        //cout << "----faithArray----" <<endl;
        //for(int i=0; i<foodGrid; i++)
        //{
            //for(int j=0; j<foodGrid; j++)
            //{
                //cout << faithArray[i][j] << " ";
            //}
            //cout <<endl;
        //}


    }

    //cout << "---faithArray 최종--" <<endl;
    //for(int i=0; i<foodGrid; i++)
    //{
        //for(int j=0; j<foodGrid; j++)
        //{
            //cout << faithArray[i][j] <<" ";
        //}
        //cout <<endl;
    //}


}

void answerDayFaithResult()
{
    int foodList[8]={0}; //1~7까지 음식 번호에 맞춰 저장하므로 8까지로 크기 설정. 인덱스 0은 비어있음. 

    //결과 
    //전체 순회하면서 각 음식에 해당하는 신앙심 저장 
    for(int row=0; row<foodGrid; row++)
    {
        for(int col=0; col < foodGrid; col++)
        {
            foodList[foodArray[row][col]] += faithArray[row][col];

        }
    }
    
    //각 음식의 신앙심 총합을 출력 
    cout << foodList[7] << " ";
    cout << foodList[3] << " ";
    cout << foodList[5] << " ";
    cout << foodList[6] << " ";
    cout << foodList[4] << " ";
    cout << foodList[2] << " ";
    cout << foodList[1] << " ";
    cout <<endl;

}

int main() {

    cin >> foodGrid >> dayCount; //NXN 신봉 음식, 며칠 //4,2

    //신봉 음식 초기화 
    for(int i=0; i<foodGrid; i++)
    {
        string foodArr;
        cin >> foodArr;

        for(int j=0; j<foodGrid; j++)
        {
            char charFood = foodArr[j];

            //신봉 음식을 정수로 
            if(charFood=='T')
            {
                foodArray[i][j]=1; //민트 우유 = 1

            }else if(charFood=='C')
            {
                foodArray[i][j]=2; //초코 우유 = 2

            }else if(charFood=='M')
            {
                foodArray[i][j]=4; //우유 = 4
            }
            //민트초코 : 3, 민트우유: 5, 초코 우유: 6, 민트초코우유: 7

            //cout << foodArray[i][j] << " ";

        }

        //cout << endl;
    }

    //신앙심 초기화 
    for(int i=0;i<foodGrid;i++)
    {
        for(int j=0; j<foodGrid; j++)
        {
            cin >> faithArray[i][j];
        }
    }

    //T일 동안
    for(int day= 0; day<dayCount; day++)
    {

        morningTime();

        lunchTime();

        dinnerTime();

        answerDayFaithResult();


    }



    return 0;
}