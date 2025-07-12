#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

#define QR_SIZE 29
#define DATA_BITS 55 * 8

int qr[QR_SIZE][QR_SIZE] = {0};
vector<int> dataStream;

void printQr();
void getDataStream(string inp);
void printArr(vector<int> arr);

int main()
{
    string inp;
    cout << "Enter your message: ";
    getline(cin, inp);

    getDataStream(inp);
    printArr(dataStream);

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
