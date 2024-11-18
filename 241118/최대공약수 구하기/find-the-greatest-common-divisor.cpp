#include <iostream>
using namespace std;

int main() {

    int n, m;
    int result;

    cin >> n >> m;

    if (m > n)
    {
        result = m % n;

        if (n == 1)
            result = 1;
    }
    else
    {
        result = n % m;

        if (m == 1)
            result = 1;
    }

    cout << result << endl;
    
    return 0;
}