#include <iostream>
using namespace std;


bool returnNum(int i)
{
    
    if (i % 2 == 0)
    {
        return false;
    }

    if (i % 10 == 5)
    {
        return false;
    }

    if (i % 3 == 0 && i % 9 != 0)
    {
        return false;
    }

    return true;
}



int main() {

    int a, b;
    int count = 0;

    cin >> a >> b;

    for (int i = a; i <= b; i++)
    {
        bool result = returnNum(i);

        if (result==true)
            count++;
    }

    cout << count << endl;

    return 0;
}