#include <iostream>
using namespace std;

int main() {

    int n, m;
    int result;

    cin >> n >> m;

    if (n > m)
    {
        while (m != 0)
        {
            result= n % m;
            n = m;
            m = result;

        }
        cout << n;
    }
    else
    {
        while (n != 0)
        {
            result = m % n;
            m = n;
            n = result;
            
        }
        cout << m;
    }
    
    
    return 0;
}