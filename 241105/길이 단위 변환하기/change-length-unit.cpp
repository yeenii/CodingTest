#include <iostream>
using namespace std;

int main() {
    double a=9.2f;
    double b = 1.3f;


    double aa = a * 30.48f;
    double bb = b * 160934;

    cout << fixed;
    cout.precision(1);

    cout << a <<"ft = " << aa <<"cm"<<endl;
    cout << b << "mi = " << bb <<"cm";

    return 0;
}