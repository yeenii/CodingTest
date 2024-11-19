#include <iostream>
#include<string>
using namespace std;

int main() {
    int n;
    int sum_num = 0;

    cin >> n;

    if (n > 9 && n < 100)
    {
        string numStr = to_string(n);

        for (char digit_char : numStr)
        {
            int num = digit_char - '0';
            sum_num += num;

        }

        if (n % 2 == 0 && sum_num % 5 == 0)
            cout << "Yes";
        else
            cout << "No";
    }
    return 0;
}