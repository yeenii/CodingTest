#include <iostream>
using namespace std;

int main() {
    int h, w;
    double b;

    cin >> h >> w;

    cout << fixed;
    cout.precision(0);

    b = (10000 * w) / (h * h);

    cout << b << endl;

    if (b >= 25)
    {
        cout << "obesity" << endl;
    }

    return 0;
}