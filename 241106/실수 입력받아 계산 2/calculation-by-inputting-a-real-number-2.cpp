#include <iostream>
using namespace std;

int main() {
    double a;

    cin >> a;

    a = a +1.5f;

    cout << fixed;
    cout.precision(2);

    cout << a;
    return 0;
}