#include<iostream>
#include<vector>
#include<algorithm>
#include<limits>

using namespace std;

//**const 반드시 붙이기
const int CONTAINER_MAX = 16;
const int EXPERIMENT_MAX = 51;

int currentContainer[CONTAINER_MAX][CONTAINER_MAX];
int newContainer[CONTAINER_MAX][CONTAINER_MAX];

int containerSize=0;
int experimentCount=0;
int r1, c1, r2, c2 = 0;

bool visited[CONTAINER_MAX][CONTAINER_MAX]; //현재 셀 방문 여부 T/F
int connectMicro[EXPERIMENT_MAX];

int drow[4] = {-1, 0, 1, 0};
int dcol[4] = {0, -1, 0, 1};

int microCellCount[EXPERIMENT_MAX]; //각 미생물 무리의 셀 개수

//dfs로 연결 여부 확인
void dfsConnectMicro(int microId, int row, int col)
{
    //방문 처리
    visited[row][col] = true;

    //상하좌우 같은 microId인지 확인
    for(int i=0; i<4; i++)
    {
        int neighRow = row + drow[i];
        int neighCol = col + dcol[i];

        //같은 id가 아니면 건너뛰기 
        if(currentContainer[neighRow][neighCol] != currentContainer[row][col])
            continue;
        
        //이미 방문한 셀이면 견너뛰기
        if(visited[neighRow][neighCol])
            continue;
        
        //배양 용기 영역을 넘어가면 건너뛰기 
        if(neighRow < 0 || neighRow >= containerSize || neighCol < 0 || neighCol >= containerSize)
            continue;
        
        dfsConnectMicro(microId, neighRow, neighCol); //이웃 셀도 방문 처리 

    }

}

void inputMicro(int microId, int r1, int c1, int r2, int c2) // 2, 2, 3, 5, 8
{
    //배양 용기에 한 무리 투입
    for(int row = r1; row < r2; row++) //2, 3, 4 - 3칸
    {
        for(int col = c1; col< c2; col++) //3, 4, 5, 6, 7 - 5칸
        {
            currentContainer[row][col] = microId; //미생물 투입 //2번
        }
    }

    //방문 여부 초기화 
    for(int row=0; row<containerSize; row++)
    {
        for(int col=0; col<containerSize; col++)
        {
            visited[row][col] = false;

        }
    }
    
    //연결 여부 초기화
    for(int id=1; id<experimentCount; id++)
    {
        connectMicro[id]=0;

    }
    

    //이어져있는지 여부 확인 
    for(int row=0; row<containerSize; row++)
    {
        for(int col=0; col<containerSize; col++)
        {
            int currentMicroId = currentContainer[row][col]; //**

            if(visited[row][col]) //**이미 방문한 경우 건너뛰기 
                continue;

            if(currentContainer[row][col]==0) //빈 셀인 경우 건너뛰기 
                continue;

            connectMicro[currentMicroId]++; //**현재 microId인 미생물 무리의 연결 여부 1 증가 

            dfsConnectMicro(currentMicroId, row, col);
        }
    }

    //연결 여부가 2이상이면 미생물 무리의 영역이 둘 이상으로 나눠진 것. 
    //연결 여부 2이상인 미생물 무리 제거 
    for(int row=0; row<containerSize; row++)
    {
        for(int col=0; col< containerSize; col++)
        {
            int currentMicroId = currentContainer[row][col];

            if(currentContainer[row][col]==0)
                continue;

            if(connectMicro[currentMicroId]>=2)
                currentContainer[row][col] = 0; 

        }
        
    }

}


pair<int, int> boundingBoxLow[EXPERIMENT_MAX]; //**
pair<int, int> boundingBoxHigh[EXPERIMENT_MAX];

vector<pair<int, int>> sortingMicro;

void changeContainer(int microId) //2
{
    //영역이 가장 넓은 무리 부터 
    // int microCellCount[id], vector<pair<int, int>> sortingMicro =  {-microSize, microId}

    //미생물 무리의 셀 개수 카운트 초기화
    for(int id=1; id<=experimentCount;id++)
    {
        microCellCount[id]=0;
    }
    
    //각 미생물 무리 셀 개수 카운트
    for(int row=0; row<containerSize; row++)
    {
        for(int col=0; col<containerSize; col++)
        {
            if(currentContainer[row][col]==0) //빈 셀일 경우 넘어가기 
                continue;

            int currentMicroId = currentContainer[row][col];
            //현재 id인 미생물 무리의 셀 개수 카운트
            microCellCount[currentMicroId]++; //12

        }
    }

    cout << microCellCount[1] <<endl;
    cout << microCellCount[2] <<endl;

    for(int id=1; id<=microId; id++)
    {
        sortingMicro.push_back({-microCellCount[id], id}); //영역 가장 넓은 무리부터 정렬

    }
    
    sort(sortingMicro.begin(), sortingMicro.end()); //정렬

    for(int i=0; i<2; i++)
    {
        cout << sortingMicro[1].first <<endl;
    }

    for(auto sortM : sortingMicro) //정렬 순 대로 
    {
        //현재 microId
        int currentMicroId = sortM.second;

        //전체 순회하면서 현재 microId인 미생물 무리의 형태 구하기 
        //경계 박스 구하기 {최소 행, 최소 열}, {최대 행, 최대 열}
        boundingBoxLow[currentMicroId] = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
        boundingBoxHigh[currentMicroId] = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

        for(int row=0; row<containerSize; row++)
        {
            for(int col=0; col<containerSize; col++)
            {
                if(currentContainer[row][col]!= currentMicroId) //현재 id인 미생물 무리가 아니면 건너뛰기
                    continue;

                boundingBoxLow[currentMicroId].first = min(boundingBoxLow[currentMicroId].first, row); //최소 행
                boundingBoxLow[currentMicroId].second = min(boundingBoxLow[currentMicroId].second, col); //최소 열
                boundingBoxHigh[currentMicroId].first = max(boundingBoxHigh[currentMicroId].first, row); //최대 행
                boundingBoxHigh[currentMicroId].second = max(boundingBoxHigh[currentMicroId].second, col); //최대 열

            }
        }

        //경계 박스로 현재 id인 미생물 무리의 행 & 열 칸 수 구하기 
        int microCellRowCount = boundingBoxHigh[currentMicroId].first - boundingBoxLow[currentMicroId].first + 1; //행 칸 수 //3
        int microCellColCount = boundingBoxHigh[currentMicroId].second - boundingBoxLow[currentMicroId].second +1; //열 칸 수 //4

        //cout << "최소 행: " << boundingBoxLow[1].first <<endl;
        //cout << "최소 열: " << boundingBoxLow[1].second <<endl;
        //cout << "최대 행: " << boundingBoxHigh[1].first <<endl;
        //cout << "최대 열: " << boundingBoxHigh[1].second <<endl;
        //cout << "microCellRowCount : "<<microCellRowCount <<endl;
        //cout << "microCellColCount : "<< microCellColCount <<endl;

        //x,y 좌표가 가장 작은 위치부터 배치 - 형태 유지하면서 
        for(int dr=0; dr< containerSize - microCellRowCount; dr++) //8-3 = 5
        {
            bool placeThisRow = false;

            for(int dc=0; dc < containerSize - microCellColCount ; dc++) //8 - 4 = 4
            {
                bool canPlace = true;

                //조건에 맞는지 확인 
                for(int r=0; r<microCellRowCount; r++)
                {
                    for(int c=0; c<microCellColCount; c++)
                    {
                        int oriRow = boundingBoxLow[currentMicroId].first + r;
                        int oriCol = boundingBoxHigh[currentMicroId].second + c;

                        //**경계 박스 안에 있지만, 다른 미생물 무리 id일 수도 있어 체크 
                        if(currentContainer[oriRow][oriCol]!= currentMicroId)
                            continue;

                        //다른 미생물 영역과 겹치는 경우, 새배양 용기로 옮기지 못함 
                        if(newContainer[dr+r][dc+c]!=0)
                        {
                            canPlace = false;
                            break;

                        }
                            
                    }

                    if(!canPlace)
                        break;
                }


                if(canPlace)
                {
                    for(int r =0; r<microCellRowCount; r++) //3
                    {
                        for(int c = 0; c<microCellColCount; c++) //4
                        {
                            int oriRow = boundingBoxLow[currentMicroId].first + r;
                            int oriCol = boundingBoxHigh[currentMicroId].second + c;

                            //**경계 박스 안에 있지만, 다른 미생물 무리 id일 수도 있어 체크 
                            if(currentContainer[oriRow][oriCol]!= currentMicroId)
                                continue;

                            newContainer[dr + r][dc+ c]=currentMicroId;
                        }
                    }

                }
                
                placeThisRow = true;

            }

            if(placeThisRow)
                break;
        } 

    }

    //기존 배양 용기에 업데이트
    for(int row=0; row<containerSize; row++)
    {
        for(int col=0; col<containerSize; col++)
        {
            currentContainer[row][col] = newContainer[row][col];
        }
    }
    

}

bool neighCell[EXPERIMENT_MAX][EXPERIMENT_MAX]; //**인접 여부 확인
int experimentMicroResult =0;
int diffA =0;
int diffB = 0; 

void drawExperimentResult(int microId)
{
    //상하좌우 맞닿은 면 있는지 확인 
    for(int row=0; row<containerSize; row++)
    {
        for(int col=0; col<containerSize; col++)
        {
            //**빈 셀은 넘어가기
            if(currentContainer[row][col]==0)
                continue;

            for(int i=0; i< 4; i++)
            {
                int neighRow = row + drow[i];
                int neighCol = col + dcol[i];

                if(currentContainer[row][col] != currentContainer[neighRow][neighCol])
                {
                    //배양 용기 영역을 넘어가면 건너뛰기 
                    if(neighRow < 0 || neighRow >= containerSize || neighCol < 0 || neighCol >= containerSize)
                        continue;

                    diffA = currentContainer[row][col];
                    diffB = currentContainer[neighRow][neighCol];

                    neighCell[diffA][diffB]= true;
                    neighCell[diffB][diffA]= true;

                }

            }
            
        }
    }

    //A-B의 영역 곱
    for(int dA = 1; dA < diffA; dA++)
    {
        for(int dB = dA +1; dB < diffB; dB++)
        {
            if(neighCell[dA][dB])
            {
                experimentMicroResult = microCellCount[dA] * microCellCount[dB];
            }
        }
    }

    cout << experimentMicroResult <<endl;

}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> containerSize >> experimentCount; //**cin에는 endl 사용하지 않음 

    //초기 배양 용기 초기화
    for(int row = 0 ; row < containerSize; row++)
    {
        for(int col=0; col< containerSize; col++)
        {
            currentContainer[row][col] = 0;
        }
    }


    for(int i = 1; i<=2; i++)
    {
        cin >> r1 >> c1 >> r2 >> c2;

        inputMicro(i, r1, c1, r2, c2);

        changeContainer(i);

        drawExperimentResult(i);

    }


    return 0;
}