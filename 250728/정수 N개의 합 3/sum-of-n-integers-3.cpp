#include <iostream>
#include<climits>

using namespace std;

const int n_max = 500;

int n, k;
int arr[n_max+1][n_max+1];
int prefix_sum[n_max+1][n_max+1];

int main() {
    cin >> n >> k; //n=3, k=2

    for(int i=0; i<=n; i++)
    {
        for(int j=0; j<=n; j++)
        {
            arr[i][j]=0;

        }
    }

    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=n; j++)
        {
            cin >> arr[i][j];

        }
    }
    
    int prefix_sum[n+1][n+1]={};

    //1. 누적합
    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=n; j++)
        {
            prefix_sum[i][j]= prefix_sum[i-1][j] + prefix_sum[i][j-1] - prefix_sum[i-1][j-1] + arr[i][j];
        }
    }

    //2. 구간합
    int max_sum =0; //최대 구간합 초기화
    for(int i=1; i<= n-k+1; i++)  //왼쪽 상단 좌표
    {
        for(int j=1; j<= n-k+1; j++)
        {
            //오른쪽 하단 좌표
            int x2 = i + k -1;
            int y2 = j + k -1;

            //구간합
            int sum = prefix_sum[x2][y2] - prefix_sum[i-1][y2] - prefix_sum[x2][j-1] + prefix_sum[i-1][j-1];

            //비교 하며 최대 구간 합 구하기 
            max_sum = max(sum, max_sum);
        }

    }
    
    cout << max_sum <<endl; //최대 구간 합 출력

    return 0;
}
