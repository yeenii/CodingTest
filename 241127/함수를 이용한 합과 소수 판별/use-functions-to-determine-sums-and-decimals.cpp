#include <iostream>
#include<math.h>
#include<string>
using namespace std;


int returnNum(int i)
{
    
    for (int j = 2; j <= sqrt(i); j++)
    {
        if (i % j == 0)
            return false;

    }

    return true;
}



int main() {

    int a, b;
    int count = 0;
    int sum;
    cin >> a >> b;

    for (int i = a; i <= b; i++)
    {
        if (returnNum(i))
        {

            string num_str = to_string(i);
            sum = 0;

            for (char digit_char : num_str)
            {
                int num = digit_char - '0';
                
                sum += num;

            }
            if (sum % 2 == 0)
            {
                count++;
            } //for문 안에 if문 작성하면 count 중복됨.
        }
       
    }

    cout << count << endl;

    return 0;
}