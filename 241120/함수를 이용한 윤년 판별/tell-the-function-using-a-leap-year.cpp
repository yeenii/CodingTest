#include <iostream>
using namespace std;

bool returnFT(int y)
{
    if(y % 4 == 0)
    {
        if (y % 100 == 0 && y % 400 != 0)
            return false;

        return true;
    }
    else
    {
        return false;
    }

}

int main() {

    int y;

    cin >> y;

    if (returnFT(y))
        cout << "true";
    else
        cout << "false";

    return 0;
}