#include <iostream>
#include<string>
using namespace std;

int main() {
    int a, b;
    int sum = 0;


    cin >> a >> b;

    for (int i = a; i <= b; i++)
    {
        string num_str = to_string(i);

        for (char digit_str : num_str)
        {
            int num = digit_str - '0';

            if (num == 3 || num == 6 || num == 9 || num%3==0)
            {
               sum += 1;
            }

        }
    }

    cout << sum << endl;

    return 0;
}