#include "funciones.h"
#include <iostream>
using namespace std;


void byteToBits(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits += ((byte >> i) & 1) ? '1' : '0';
}

string encodeMethod1_fixed(const string &bits, int n) {
    int totalBits = bits.size();
    int bloques = totalBits / n;
    if (bloques == 0) return "";

    string out(bloques * n, '0');

    for (int b = 0; b < bloques; ++b) {
        int startCur = b * n;

        if (b == 0) {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
            continue;
        }

        int startPrev = (b - 1) * n;
        int unos = 0, ceros = 0;
        for (int i = 0; i < n; ++i)
            (bits[startPrev + i] == '1') ? ++unos : ++ceros;

        if (unos == ceros) {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
        }
        else if (ceros > unos) {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (i % 2 == 1) ? ((bits[startCur + i] == '0') ? '1' : '0') : bits[startCur + i];
        }
        else {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (i % 3 == 2) ? ((bits[startCur + i] == '0') ? '1' : '0') : bits[startCur + i];
        }
    }
    return out;
}

string encodeMethod2(const string &bits, int n) {
    int totalBits = bits.size();
    int bloques = totalBits / n;
    string out(bloques * n, '0');

    for (int b = 0; b < bloques; ++b) {
        int start = b * n;
        if (n == 1) {
            out[start] = bits[start];
            continue;
        }

        out[start] = bits[start + n - 1];
        for (int i = 1; i < n; ++i)
            out[start + i] = bits[start + i - 1];
    }
    return out;
}
