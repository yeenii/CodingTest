#include <iostream>
#include<math.h>
using namespace std;

bool isPrime(int n)
{
    //
    if (n <= 1)
        return false;

    for (int j = 2; j <= sqrt(n); j++) //2, 25 //2,36
    {
        if (n % j == 0) // 18...0 = 36 % 2
            return false;
    }
    return true; //12...1 = 25 % 2 

}

int IsPrime(int a, int b)
{
    int result=0;


    for (int i = a; i <= b; i++)//a와 b 사이의 소수값
    {
        
        if (isPrime(i))
        {
            result += i; //소수 값만 더하기
            cout << i << endl;
        }
        

    }
    return result;
}

int main() {
    int a, b;

    cin >> a >> b;

    int isPrime = IsPrime(a, b);

    cout << "소수값: " << isPrime << endl;

    return 0;
}