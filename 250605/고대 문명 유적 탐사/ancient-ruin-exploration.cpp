#include <iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<tuple>
using namespace std;

const int K = 15; // 1 <= k <= 10
const int M = 330; // 10 <= M <= 300

int k, m =0; //탐사 반복 횟수, 유물 조각 개수 
int historySpace[5][5]; //유적지 정보 저장
int pieceWall[M]; //m 개의 유물 조각 번호 
int centerPoint[5][5]; //3x3 격자 중심 좌표인 곳은 1로 설정 
//int grid3x3[9][5][5];
vector<pair<int, pair<int,int>>> grid3x3[9]; //{값, {row,col}}

int tempGrid[5][5]; //회전한 배열 임시 저장
queue<int> pieceWall_que; //유적 벽면에 써있는 숫자 저장한 큐

//BFS
bool visited[5][5]; //방문 여부 
int drow[4] = {-1, 1, 0, 0};
int dcol[4] = {0, 0, -1, 1};

/* 
    구조체 생성 -> 우선 순위 별 정렬 
    (1) 유물 가치 가장 큰 값 
    (2) 회전 각도 가장 작은 구간 
    (3) 중심 좌표의 열이 가장 작은 구간 
    (4) 중심 좌표의 행이 가장 작은 구간
*/
struct RotationResult {
    int score; //유물가치
    int angle; //회전각도: 90, 180, 270 
    int row; // 열 - 오름차순
    int col; //행 - 오름차순
    int index; //구간 0~9
};

vector<RotationResult> results; // 유물 가치 저장 
vector<pair<int,int>> spaceZero; //0인 위치 저장 

vector<pair<int, int>> bfsConnectPiece( int val,int row, int col)
{
    queue<pair<int, int>> que;
    vector<pair<int, int>> group; //같은 조각이 인접해 있는 그룹 row, col 좌표 저장 

    //cout << "val: "<< val << "row: "<<row << "col: "<<col <<endl;

    que.push({row, col});
    visited[row][col]=true;
    group.push_back({row, col});

    while(!que.empty())
    {
        int currentRow = que.front().first;
        int currentCol = que.front().second;

        que.pop();

        for(int i=0; i<4; i++)//상하좌우 
        {
            int nextRow = currentRow + drow[i];
            int nextCol = currentCol + dcol[i];

            //격자 밖일 경우 건너뛰기
            if(nextRow < 0 || nextRow >= 5 || nextCol <0 || nextCol >=5)
                continue;
            
            //방문 여부 
            if(visited[nextRow][nextCol])
                continue;
            
            if(tempGrid[nextRow][nextCol]==val)
            {
                group.push_back({nextRow,nextCol}); //인접한 같은 조각 종류 저장 
                que.push({nextRow,nextCol});
                visited[nextRow][nextCol] = true; //큐에 푸시하면서 방문처리 
            }

            
            
        }

    }

    //그룹 확인 
    //for(int i=0; i<group.size(); i++)
        //cout << "x: "<<group[i].first <<"y: " << group[i].second <<endl;
    //cout <<endl;

    return group;
    
}

int repeatGet()
{
    vector<pair<int,int>> result;
    int score =0;

    //방문여부 초기화
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            visited[i][j]=false;
        }
    }

    // tempGrid 초기화
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tempGrid[i][j] = historySpace[i][j];
        }
    }


    //2. BFS로 인접한 것 다 구하기 
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if (visited[i][j])
                continue;


            //전체 셀 돌면서, 해당 셀과 같은 종류의 셀을 구하고, 그 셀 들이 3개 이상일 때 유물 가치 획득. 
            result = bfsConnectPiece(tempGrid[i][j],i,j); //값, row, col 넘겨줌 

            if(result.size() >=3)
            {
                score+= result.size(); //인접한 조각 3개 이상이면 저장 

                for(int d=0; d<result.size(); d++) //셀 제거 
                {
                    int deleteRow = result[d].first;
                    int deleteCol = result[d].second;
                    
                    tempGrid[deleteRow][deleteCol]=0;
                }

            }           
         
        }
    }

    // 유물 제거 이후 상태를 historySpace에 반영
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            historySpace[i][j] = tempGrid[i][j];
        }
    }

    return score;
}

//90도 회전 
int turnGrid_90(int count, int centerRow, int centerCol, bool isApply) 
{
    //1. 3x3 회전
    //tempGrid를 historySpace로 초기화
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            tempGrid[i][j]=historySpace[i][j];
        }
    }

    //grid3x3을 90도 회전
    for(int i=0; i<9; i++)
    {

        int val = grid3x3[count][i].first;

        //중심 위치의 상대 좌표 
        int r = grid3x3[count][i].second.first - centerRow; //-1, 0, +1
        int c = grid3x3[count][i].second.second - centerCol;

        //3x3 상대좌표 회전 공식 : (r,c) -> (c, -r)
        int newRow = centerRow+c; 
        int newCol = centerCol-r;

        if(newRow<0 || newRow>=5 || newCol <0 || newCol >=5) 
            continue;

        tempGrid[newRow][newCol]= val; 

    }

    vector<pair<int,int>> result;
    int score =0;

    //방문여부 초기화
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            visited[i][j]=false;
        }
    }

    //2. BFS로 인접한 것 다 구하기 
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if (visited[i][j])
                continue;

            //전체 셀 돌면서, 해당 셀과 같은 종류의 셀을 구하고, 그 셀 들이 3개 이상일 때 유물 가치 획득. 
            result = bfsConnectPiece(tempGrid[i][j],i,j); //값, row, col 넘겨줌 

            if(result.size() >=3)
            {
                score+= result.size(); //인접한 조각 3개 이상이면 저장 

                for(int d=0; d<result.size(); d++) //셀 제거 
                {
                    int deleteRow = result[d].first;
                    int deleteCol = result[d].second;
                    
                    tempGrid[deleteRow][deleteCol]=0;
                }

            }           
         
        }
    }

    //tempGrid90[i][j]에 tempGrid의 값 저장 
    //만약에 90도 회전이 가장 큰 유물 가치를 갖고 있다면, tempGrid90에 있는 값 리턴 | 아니라면, 넘기지 않음
    //temp90을 0으로 초기화
    if(isApply)
    {
        for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                historySpace[i][j]=tempGrid[i][j];
            }
        }
    }

    return score;

}

int turnGrid_180(int count, int centerRow, int centerCol,bool isApply) 
{
    //tempGrid를 historySpace로 초기화
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            tempGrid[i][j]=historySpace[i][j];
        }
    }

    //grid3x3을 90도 회전
    for(int i=0; i<9; i++)
    {

        int val = grid3x3[count][i].first;

        //중심 위치의 상대 좌표 
        int r = grid3x3[count][i].second.first - centerRow; //-1, 0, +1
        int c = grid3x3[count][i].second.second - centerCol;

        //3x3 상대좌표 180도 회전 공식 : (r,c) -> (-r,-c)
        int newRow = centerRow-r; 
        int newCol = centerCol-c;

        if(newRow<0 || newRow>=5 || newCol <0 || newCol >=5) 
            continue;

        tempGrid[newRow][newCol]= val; 

    }


    vector<pair<int,int>> result;
    int score =0;

    //방문여부 초기화
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            visited[i][j]=false;
        }
    }

    //2. BFS로 인접한 것 다 구하기 
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if (visited[i][j])
                continue;

            //전체 셀 돌면서, 해당 셀과 같은 종류의 셀을 구하고, 그 셀 들이 3개 이상일 때 유물 가치 획득. 
            result = bfsConnectPiece(tempGrid[i][j],i,j); //값, row, col 넘겨줌 

            if(result.size() >=3)
            {
                score+= result.size(); //인접한 조각 3개 이상이면 저장 

                for(int d=0; d<result.size(); d++) //셀 제거 
                {
                    int deleteRow = result[d].first;
                    int deleteCol = result[d].second;
                    
                    tempGrid[deleteRow][deleteCol]=0;
                }

            }           
         
        }
    }

    //tempGrid180[i][j]에 tempGrid의 값 저장 
    //만약에 180도 회전이 가장 큰 유물 가치를 갖고 있다면, tempGrid180에 있는 값 리턴 | 아니라면, 넘기지 않음
    //temp180을 0으로 초기화
    if(isApply)
    {
        for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                historySpace[i][j]=tempGrid[i][j];
            }
        }
    }
    
    return score; //유물 가치 

}

int turnGrid_270(int count, int centerRow, int centerCol,bool isApply) 
{
    //tempGrid를 historySpace로 초기화
    //int tempGrid[5][5];
    int temp3x3[3][3];

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            tempGrid[i][j]=historySpace[i][j];
        }
    }

    //grid3x3을 90도 회전
    for(int i=0; i<9; i++)
    {

        int val = grid3x3[count][i].first;

        //중심 위치의 상대 좌표 
        int r = grid3x3[count][i].second.first - centerRow; //-1, 0, +1
        int c = grid3x3[count][i].second.second - centerCol;

        //3x3 상대좌표 180도 회전 공식 : (r,c) -> (-c, r)
        int newRow = centerRow-c; 
        int newCol = centerCol+r;

        if(newRow<0 || newRow>=5 || newCol <0 || newCol >=5) 
            continue;

        tempGrid[newRow][newCol]= val; 

    }

    vector<pair<int,int>> result;
    int score =0;

    //방문여부 초기화
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            visited[i][j]=false;
        }
    }

    //2. BFS로 인접한 것 다 구하기 
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if (visited[i][j])
                continue;

            //전체 셀 돌면서, 해당 셀과 같은 종류의 셀을 구하고, 그 셀 들이 3개 이상일 때 유물 가치 획득. 
            result = bfsConnectPiece(tempGrid[i][j],i,j); //값, row, col 넘겨줌 

            if(result.size() >=3)
            {
                score+= result.size(); //인접한 조각 3개 이상이면 저장 

                for(int d=0; d<result.size(); d++) //셀 제거 
                {
                    int deleteRow = result[d].first;
                    int deleteCol = result[d].second;
                    
                    tempGrid[deleteRow][deleteCol]=0;
                }

            }           
         
        }
    }

    //tempGrid270[i][j]에 tempGrid의 값 저장 
    //만약에 270도 회전이 가장 큰 유물 가치를 갖고 있다면, tempGrid270에 있는 값 리턴 | 아니라면, 넘기지 않음
    //temp270을 0으로 초기화
    if(isApply)
    {
        for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                historySpace[i][j]=tempGrid[i][j];
            }
        }
    }

    return score; //유물 가치

}


void fillZeroSpace()
{
    // (1) 0인 위치의 row, col 저장 
    spaceZero.clear();

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if(historySpace[i][j]==0)
            {
                spaceZero.push_back({j,-i}); //row, col 저장 
            }
        }
    }

    // (2) 정렬 : 열 번호 작은 순 - 행번호 작은 순
    sort(spaceZero.begin(), spaceZero.end()); 
    
    // (3) 유적의 벽면에 써있는 숫자를 순서대로 채워넣음 
    for(int i=0; i<spaceZero.size(); i++)
    {
        if (pieceWall_que.empty()) 
            return;

        int num = pieceWall_que.front();
        historySpace[-spaceZero[i].second][spaceZero[i].first]= num;
        pieceWall_que.pop();

    }
}

int main() {
    
    cin >> k >> m; //탐사 반복 횟수, 유물 조각 개수 
    //cout << k <<" " << m <<endl;

    // 5x5 유적지 
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            cin >> historySpace[i][j];
            //cout << historySpace[i][j] <<" "; 
        }
        //cout <<endl;
    }
    
    //유적의 벽면에 써있는 숫자 
    for(int i=0; i<m; i++)
    {
        cin >> pieceWall[i];
        //cout <<pieceWall[i] << " ";
    }
    //cout <<endl;

    for(int i=0; i<m; i++)
    {
        pieceWall_que.push(pieceWall[i]); //유적의 벽면에 써있는 숫자 입력받은 것을 que에 저장 
    }

    int maxArtifact; //유물 가치의 총합

    for(int t=0; t<k; t++)//k번 턴
    {
        maxArtifact =0; //초기화

        if(pieceWall_que.empty())
            break;

        //grid3x3 초기화 
        for(int i=0; i<9; i++)
        {
            grid3x3[i].clear();
        }

        //1. 탐사 진행 
        // 가능한 3X3 격자 중심 좌표 선택 & 90, 180, 270도 회전
        //(1) 3x3 격자 중심 좌표 구하기 
        for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                centerPoint[i][j]=0; //초기화  
            }
        }

        for(int i=1; i<=3; i++)
        {
            for(int j=1; j<=3; j++)
            {
                centerPoint[i][j]=1; //중심 좌표 가능한 곳만 1로 표시.
            }
        }

        //(2) 중심 좌표 기준으로 3x3 그리드 구하고, 배열 저장 
        int count=0;
        for(int i=0; i<5; i++)
        {
            for(int j=0; j<5; j++)
            {
                if(centerPoint[i][j]==1)
                {
                    int centerPointRow = i;
                    int centerPointCol = j;
                    //cout << "i: " << i <<" j: " << j <<endl;
                
                    for(int dr=centerPointRow-1; dr<=centerPointRow+1; dr++)
                    {
                        //cout << "count: "<< count <<endl;
                        for(int dc = centerPointCol-1; dc<=centerPointCol+1; dc++)
                        {
                            //grid3x3[count][dr][dc] = historySpace[dr][dc]; //값
                            grid3x3[count].push_back({historySpace[dr][dc],{dr,dc}});
                            //cout << grid3x3[count][dr][dc] <<" ";
                        }
                        //cout <<endl;
                    }
                    count++;
                    //cout <<endl;
                }
            }
        }


        //(3) grid3x3을 90도, 180도, 270도 회전하고 
        //(4) 유물 가치 가장 큰 값 구하기 
        // isApply -> true : 유물 가치 가장 큰 값 리턴, false : 아님 

        //results 초기화
        results.clear();

        for(int i=0; i<9;i++) //9개의 3x3 그리드 구간 
        {
            //중앙 좌표 구하기 
            int centerRow = grid3x3[i][4].second.first;
            int centerCol = grid3x3[i][4].second.second;

            //90도 회전
            int artifact90 = turnGrid_90(i, centerRow, centerCol, false);

            //180도 회전 
            int artifact180 = turnGrid_180(i, centerRow, centerCol, false);

            //270도 회전 
            int artifact270 = turnGrid_270(i, centerRow, centerCol, false);

            //유물 가치 가장 큰 회전 구하기위해 results에 모든 구간 저장 
            results.push_back({artifact90, 90, centerCol, centerRow, i}); //90도 //점수는 내림차순
            results.push_back({artifact180,180, centerCol, centerRow, i}); //180도
            results.push_back({artifact270,270, centerCol, centerRow, i}); //270도
        }

        //우선 순위 별 정렬을 통해 유물 가치 가장 큰 회전 구함
        //정렬
        //구조체는 operator를 자동 적용해주지 않기 때문에 에러남. -> sort()에서 비교해줘야 함 
        sort(results.begin(), results.end(), [](const RotationResult& a, const RotationResult& b){
        if(a.score != b.score) return a.score > b.score; //내림차순
        if(a.angle != b.angle) return a.angle < b.angle; //오름차순
        if(a.col != b.col) return a.col < b.col; //오름차순 
        if(a.row != b.row) return a.row < b.row; //오름차순
        //index는 정렬할 필요 없음
        }); 

        RotationResult best = results[0]; //가장 유물가치가 큰 값 

        //모든 유물 획득 과정에서 유물을 획득하지 못하였기 때문에 탐사 즉시 종료 
        if(best.score==0)
            break;
    
        //최대 유물 가치 획득 값 저장 
        maxArtifact += best.score;

        //유물 가치 가장 큰 값의 tempGrid를 적용
        if(best.angle == 90)
            turnGrid_90(best.index, best.row, best.col, true);
        else if(best.angle==180)
            turnGrid_180(best.index, best.row, best.col, true);
        else
            turnGrid_270(best.index, best.row, best.col, true);

        //사라진 위치에 새로운 조각 생성 
        fillZeroSpace();

        //2. 유물 연쇄 획득 -  더이상 새로운 조각이 생성X & 조각이 3개 이상 연결되지 않을 때 까지 반복 
        //bfs로 인접한 조각들 연결해서 최대 유적 가치 구하고
        while(true)
        {
            int reScore = repeatGet(); //유물 가치 획득 개수 

            if(pieceWall_que.empty()|| reScore==0)
                break;
        
            fillZeroSpace(); //사라진 부분 채우기 

            maxArtifact+= reScore;
        }
    
        cout << maxArtifact <<" "; 
    }
    cout <<endl;

    
        

    //1~2까지 = 1턴.
    //k번 턴 했을 때, 각 턴마다 획득한 유물의 가치의 총합을 출력 

    return 0;
        
}