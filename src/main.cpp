#include "qr.h"
#include <iostream>

using namespace std;

int main()
{
    string inp;
    cout << "Enter your message: ";
    getline(cin, inp);

    Qr qr(inp);

    qr.print_qr();
    qr.generate_svg("qr.svg");

    return 0;
}