#include <iostream>
using namespace std;

int main() {
    double n;

    cin >> n;

    n = n * 30.48f;

    cout << fixed;
    cout.precision(1);

    cout << n;


    return 0;
}