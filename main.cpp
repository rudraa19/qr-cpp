#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

#define QR_SIZE 29
#define DATA_BITS 55 * 8
#define EC_BITS 15 * 8

int qr[QR_SIZE][QR_SIZE] = {0};
vector<int> dataStream;
vector<int> ecStream;
int divisor[] = {1, 29, 196, 111, 163, 112, 74, 10, 105, 105, 139, 132, 151, 32, 134, 26};

void printQr();
void getDataStream(string inp);
void printArr(vector<int> arr);
int gfMul(int a, int b);
void polynomialDivide();

int main()
{
    string inp;
    cout << "Enter your message: ";
    getline(cin, inp);

    getDataStream(inp);
    printArr(dataStream);

    polynomialDivide();
    cout << endl;
    printArr(ecStream);

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

void getDataStream(string inp)
{
    dataStream.push_back(0);
    dataStream.push_back(1);
    dataStream.push_back(0);
    dataStream.push_back(0);

    string temp = bitset<8>(inp.length()).to_string();
    for (char i : temp)
    {
        dataStream.push_back(i - '0');
    }

    for (char i : inp)
    {
        string t = bitset<8>(i).to_string();
        for (char j : t)
        {
            dataStream.push_back(j - '0');
        }
    }

    dataStream.push_back(0);
    dataStream.push_back(0);
    dataStream.push_back(0);
    dataStream.push_back(0);

    bool flag = false;
    for (int i = dataStream.size(); i < DATA_BITS; i += 8)
    {
        if (!flag)
        {
            dataStream.push_back(1);
            dataStream.push_back(1);
            dataStream.push_back(1);
            dataStream.push_back(0);
            dataStream.push_back(1);
            dataStream.push_back(1);
            dataStream.push_back(0);
            dataStream.push_back(0);
        }
        else
        {
            dataStream.push_back(0);
            dataStream.push_back(0);
            dataStream.push_back(0);
            dataStream.push_back(1);
            dataStream.push_back(0);
            dataStream.push_back(0);
            dataStream.push_back(0);
            dataStream.push_back(1);
        }
        flag = !flag;
    }
}

int gfMul(int a, int b)
{
    int result = 0;
    while (b)
    {
        if (b & 1)
            result ^= a;
        a <<= 1;
        if (a & 0x100)
            a ^= 0x11D;
        b >>= 1;
    }
    return result;
}

void polynomialDivide()
{
    vector<int> messageBytes;
    for (int i = 0; i < dataStream.size(); i += 8)
    {
        int byte = 0;
        for (int j = 0; j < 8; j++)
        {
            byte = (byte << 1) | dataStream[i + j];
        }
        messageBytes.push_back(byte);
    }

    vector<int> remainder = messageBytes;
    while (remainder.size() < messageBytes.size() + EC_BITS / 8)
    {
        remainder.push_back(0);
    }

    int divisorLen = EC_BITS / 8 + 1;
    for (int i = 0; i < messageBytes.size(); i++)
    {
        int coef = remainder[i];
        if (coef != 0)
        {
            for (int j = 0; j < divisorLen; j++)
            {
                remainder[i + j] ^= gfMul(divisor[j], coef);
            }
        }
    }

    for (int i = messageBytes.size(); i < remainder.size(); i++)
    {
        string bits = bitset<8>(remainder[i]).to_string();
        for (int j = 0; j < 8; j++)
        {
            ecStream.push_back(bits[j] - '0');
        }
    }
}

void printArr(vector<int> arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i];
        if ((i + 1) % 8 == 0)
        {
            cout << " ";
        }
        if ((i + 1) % (8 * 5) == 0)
        {
            cout << endl;
        }
    }
    cout << endl;
}
