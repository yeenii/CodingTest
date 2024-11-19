#include <iostream>
#include<string>
using namespace std;

int returnVal(int a, int b)
{
    int sum=0;

    for (int i = a; i <= b; i++)
    {
        

        if (i % 3 == 0)
        {
            sum++;
            continue; //3의 배수인 숫자라면 각 자리를 검사할 필요가 없으니 다음으로 바로 넘어감.
        }

        string num_str = to_string(i);

        for (char digit_str : num_str)
        {
            
            int num = digit_str - '0';

            if (num == 3 || num == 6 || num == 9)
            {
                sum++;
                break; //3, 6, 9 중에 하나를 포함하면 바로 루프 종료. 더이상 각 자리 검사하지 않고 그 다음으로 넘어감.
            }
            
            

        }
    }

    return sum;
}

int main() {
    int a, b;


    cin >> a >> b;


    int sum =returnVal(a, b);


    cout << sum << endl;

    return 0;
}