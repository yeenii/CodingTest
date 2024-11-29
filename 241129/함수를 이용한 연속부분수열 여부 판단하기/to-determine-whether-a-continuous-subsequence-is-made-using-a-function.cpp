#include <iostream>
#include<math.h>
#include<string>
using namespace std;

int num1[100];
int num2[100];

bool IsSame(int i, int n2)
{
    //n2만큼 돌면서
    for (int j = 0; j < n2; j++)
    {
        if (num1[i + j] != num2[j]) // num1의 첫번째 값과 num2의 0~n2-1까지의 값이
        {
            return false; // 다르면 false 리턴
        } // 같으면 그다음 for문 진행
    }
    return true; //n2 크기까지 도는 동안 다르지 않으면 연속하는 거임
}

bool IsSubsequence(int n1, int n2)
{   
    //n2에 +1만큼 돌음
    for (int i = 0; i < n1 - n2; i++)
    {
        if (IsSame(i, n2))
            return true;
    }
    return false;
}

int main() {

    int n1, n2;

    cin >> n1 >> n2;

    for (int i = 0; i < n1; i++)
    {
        cin >> num1[i];
    }

    for (int i = 0; i < n2; i++)
    {
        cin >> num2[i];
    }

    if (IsSubsequence(n1, n2))
        cout << "Yes";
    else
        cout << "No";

    
    return 0;
}