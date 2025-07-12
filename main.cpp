#include <iostream>

using namespace std;

#define QR_SIZE 29

int qr[QR_SIZE][QR_SIZE] = {0};

void printQr();

int main()
{
    printQr();
    return 0;
}

void printQr()
{
    for (int i = 0; i < QR_SIZE; i++)
    {
        for (int j = 0; j < QR_SIZE; j++)
        {
            cout << qr[i][j] << " ";
        }
        cout << endl;
    }
}