#include <iostream>
using namespace std;

int main() {
    int a, b,c;

    cin >> a >> b >> c;

    int result = min(a, min(b,c));
    
    cout << result <<endl;
    
    return 0;
}