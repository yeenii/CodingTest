#include <iostream>
using namespace std;

int main() {

    int sum=0;
    int n;

    cin >> n;

    for (int i = 1; i <= n; i++)
    {
        sum += i;

    }
    int result = sum / 10;
    cout << result << endl;
    return 0;
}