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
int constantPattern[][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 22}, {0, 23}, {0, 24}, {0, 25}, {0, 25}, {0, 25}, {0, 26}, {0, 27}, {0, 28}, {1, 0}, {1, 6}, {1, 22}, {1, 28}, {2, 0}, {2, 2}, {2, 3}, {2, 4}, {2, 6}, {2, 8}, {2, 22}, {2, 24}, {2, 25}, {2, 26}, {2, 28}, {3, 0}, {3, 2}, {3, 3}, {3, 4}, {3, 6}, {3, 22}, {3, 24}, {3, 25}, {3, 26}, {3, 28}, {4, 0}, {4, 2}, {4, 3}, {4, 4}, {4, 6}, {4, 22}, {4, 24}, {4, 25}, {4, 26}, {4, 28}, {5, 0}, {5, 6}, {5, 22}, {5, 28}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 8}, {6, 10}, {6, 12}, {6, 14}, {6, 16}, {6, 18}, {6, 20}, {6, 22}, {6, 23}, {6, 24}, {6, 25}, {6, 26}, {6, 27}, {6, 28}, {7, 8}, {8, 0}, {8, 1}, {8, 2}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}, {10, 6}, {12, 6}, {14, 6}, {16, 6}, {18, 6}, {20, 6}, {20, 20}, {20, 21}, {20, 22}, {20, 23}, {20, 24}, {21, 8}, {21, 20}, {21, 24}, {22, 0}, {22, 1}, {22, 2}, {22, 3}, {22, 4}, {22, 5}, {22, 6}, {22, 8}, {22, 20}, {22, 22}, {22, 24}, {23, 0}, {23, 6}, {23, 8}, {23, 20}, {23, 24}, {24, 0}, {24, 2}, {24, 3}, {24, 4}, {24, 6}, {24, 8}, {24, 20}, {24, 21}, {24, 22}, {24, 23}, {24, 24}, {25, 0}, {25, 2}, {25, 3}, {25, 4}, {25, 6}, {26, 0}, {26, 2}, {26, 3}, {26, 4}, {26, 6}, {26, 8}, {27, 0}, {27, 6}, {27, 8}, {28, 0}, {28, 1}, {28, 2}, {28, 3}, {28, 4}, {28, 5}, {28, 6}, {28, 8}};

void printQr();
void getDataStream(string inp);
void printArr(vector<int> arr);
int gfMul(int a, int b);
void polynomialDivide();
void assignConstantPattern();

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

    assignConstantPattern();

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

void assignConstantPattern()
{
    for (auto c : constantPattern)
    {
        qr[c[0]][c[1]] = 1;
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
