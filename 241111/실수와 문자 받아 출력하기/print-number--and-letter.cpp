#include <iostream>
#include<string>
using namespace std;

int main() {

    double a, b;
    string s;

    cout << fixed;
    cout.precision(2);

    cin >> s;

    cin >> a;
    cin >> b;

    cout << s <<endl;
    cout << a << endl << b;

    return 0;
}