#include <iostream>
using namespace std;

int main() {
    
    int n,m;
    int result;

    cin >> n >> m;

    if(m>n)
    {
        result=m%n;
    }
    else
    {
        result=n%m;
    }

    cout << result <<endl;
    return 0;
}