#ifndef QR_H
#define QR_H

#include <vector>
#include <string>

using namespace std;

#define QR_SIZE 29
#define DATA_BITS 55 * 8
#define EC_BITS 15 * 8
#define QUIET_ZONE 4

class Qr
{
private:
    string msg;

    int qr[QR_SIZE][QR_SIZE];
    vector<int> data_stream;
    vector<int> ec_stream;

    static const int divisor[16];
    static const int constant_pattern[][2];
    static const int variable_slots[][2];
    static const int mask[][2];

    void get_data_stream(string inp);
    int gf_mul(int a, int b);
    void polynomial_divide();
    void assign_constant_pattern();
    void fill_variable_slots();
    void assign_mask();

public:
    Qr(string msg);
    void print_qr();
    void generate_svg(string output);
};

#endif