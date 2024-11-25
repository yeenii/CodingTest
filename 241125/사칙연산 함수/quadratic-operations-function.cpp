#include <iostream>
using namespace std;

int main() {

    int a, b;
    char c;

    cin >> a >> c >> b;

    if (c == '+')
    {
        cout << a << " + " << b << " = " << a + b << endl;
    }
    else if (c == '-')
    {
        cout << a << " - " << b << " = " << a - b << endl;
    }
    else if (c == '/')
    {
        cout << a << " / " << b << " = " << a / b << endl;
    }
    else if (c == '*')
    {
        cout << a << " * " << b << " = " << a * b << endl;
    }
    else
    {
        cout << "False" << endl;
    }

    return 0;
}