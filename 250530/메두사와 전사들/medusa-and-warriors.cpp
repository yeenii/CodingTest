#include <iostream>
using namespace std;

const int N = 60; //4 <= N <= 50
const int M = 310; // 0<= M <= 300

int villageSize =0;
int warriorSize =0;
int road[N][N]; 
pair<int, int> warriorsPosition[M];

int main() {
    
    ios:sync_with_stdio(false);
    cin.tie(NULL);

    //마을 크기 N, 전사의 수 M이 공백을 사이에 두고 입력 
    //정수로 입력되니까 공백 고려X
    cin >> villageSize >> warriorSize;


    int medusaHomeRow, medusaHomeCol =0;
    int parkRow, parkCol=0;
    //메두사의 집의 위치 정보 (s_r, s_c),  공원의 위치 정보 (e_r, e_c) 입력 
    cin >> medusaHomeRow >> medusaHomeCol >> parkRow >> parkCol;

    int a1r, a1c, a2r, a2c, a3r, a3c, a4r, a4c =0;
    //M명의 전사들의 좌표 (a1r, a1c), (a2r, a2c), (a3r, a3c), (a4r, a4c) 입력
    for(int i=0; i<warriorSize; i++)
    {
        cin >> warriorsPosition[i].first >> warriorsPosition.second;
        cout << warriorsPosition[i].first << " " << warriorsPosition.second << " ";
    }
    
    
    //N 줄에 걸쳐 마을의 도로 정보 입력. 도로 O = 0, 도로 X = 1
    for(int i=0; i< villageSize; i++)
    {
        for(int j=0; j<villageSize; j++)
        {
            cin >> road[i][j];
        }

    }

    while(true) //메두사가 공원에 도달할 때까지 반복
    {
        // 1. 메두사 이동 
        medusaMove(); 

        //2. 메두사 시선 
        medusaEye(); 

        //3. 전사들의 이동 
        warriorMove(); 
        
        //4. 전사의 공격  
        warriorAttack(); 

        //메두사가 공원에 도착하면 0을 출력, 그 전까지는
        // 모든 전사가 이동한 거리의 합, 메두사로 인해 돌이된 전사 수, 메두사를 공격한 전사의 수 출력

        //메두사 집-공원까지 이어지는 도로 존재하지 않으면 -1 출력

    }


    return 0;
}