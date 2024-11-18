#include <iostream>
using namespace std;


int highVal(int n, int m)
{
    int result;

    if (n > m)
    {
        while (m != 0)
        {
            result = n % m;
            n = m;
            m = result;

        }
        return n;
    }
    else
    {
        while (n != 0)
        {
            result = m % n;
            m = n;
            n = result;

        }
        return m;
    }
}
int main() {

    int n, m;
    
    cin >> n >> m;

    int hv = highVal(n, m);
    
    int result =n* m / hv;
    
    cout << result;
    
    return 0;
}