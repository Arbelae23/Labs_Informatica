#include "funciones.h"
#include <iostream>
#include <fstream>
using namespace std;

// FUNCIONES DE CODIFICADOR

// Convierte un byte en 8 caracteres 0 / 1 y los agrega a un string
void byteToBits(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits += ((byte >> i) & 1) ? '1' : '0';
}

// Método 1 - Codificación por bloques de n bits
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

// Método 2 - Codificación por rotación dentro de cada bloque
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


// FUNCIONES DE DECODIFICADOR CON STRING

// Convierte un byte en su representación binaria (8 bits) y los agrega a un string
void byteToBits_dec(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits += ((byte >> i) & 1) ? '1' : '0';
}

// Convierte una secuencia de bits (a partir de una posición) en un byte
unsigned char bitsToByte_dec(const string &bits, int start) {
    unsigned char b = 0;
    for (int i = 0; i < 8; ++i)
        b = (b << 1) | (bits[start + i] - '0');
    return b;
}

// Metodo 1 - Decodificacion de bloques de n bits
string decodeMethod1(const string &bits, int n) {
    int totalBits = static_cast<int>(bits.size());
    int bloques = totalBits / n;
    if (bloques == 0) return string();

    string out(bloques * n, '0');

    for (int b = 0; b < bloques; ++b) {
        int startCur = b * n;

        if (b == 0) {
            // Primer bloque: invertir todos los bits
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
            continue;
        }

        // Contar unos y ceros del bloque anterior
        int startPrev = (b - 1) * n;
        int unos = 0, ceros = 0;
        for (int i = 0; i < n; ++i)
            (out[startPrev + i] == '1') ? ++unos : ++ceros;

        // Reglas de decodificacion segun cantidad de unos y ceros
        if (unos == ceros) {
            // Caso 1: invertir todos
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
        } else if (ceros > unos) {
            // Caso 2: invertir cada bit en posicion impar
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (i % 2 == 1)
                                        ? ((bits[startCur + i] == '0') ? '1' : '0')
                                        : bits[startCur + i];
        } else {
            // Caso 3: invertir cada bit en posicion multiplo de 3
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (i % 3 == 2)
                                        ? ((bits[startCur + i] == '0') ? '1' : '0')
                                        : bits[startCur + i];
        }
    }
    return out;
}

// Metodo 2 - Decodificacion por rotacion inversa dentro de cada bloque
string decodeMethod2(const string &bits, int n) {
    int totalBits = static_cast<int>(bits.size());
    int bloques = totalBits / n;
    string out(bloques * n, '0');

    for (int b = 0; b < bloques; ++b) {
        int start = b * n;
        if (n == 1) {
            out[start] = bits[start];
            continue;
        }
        // Desplazar bits una posicion hacia la izquierda (rotacion inversa)
        for (int i = 0; i < n - 1; ++i)
            out[start + i] = bits[start + i + 1];
        out[start + n - 1] = bits[start];
    }
    return out;
}

// Convierte una cadena de '0'/'1' a bytes reales (string con caracteres)
string convertirBitsABytes_string(const string &bits) {
    int outBytes = bits.size() / 8;
    string outBuffer;
    outBuffer.reserve(outBytes);

    for (int i = 0; i < outBytes; ++i) {
        unsigned char b = 0;
        for (int j = 0; j < 8; ++j)
            b = (b << 1) | (bits[i * 8 + j] - '0');
        outBuffer.push_back(static_cast<char>(b));
    }
    return outBuffer;
}
//FUNCIONES CLONADAS DE LOS METODOS

//conertimos 1 byte a cadena de caracteres
void convertirByteABits_aplicacion(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits.push_back(((byte >> i) & 1) ? '1' : '0');
}


// convertimos 1 cadena de caractes a ascci agrupando de 8
string convertirBitsABytes_aplicacion(const string &bits) {
    int outBytes = bits.size() / 8;
    string outBuffer;
    outBuffer.reserve(outBytes);

    for (int i = 0; i < outBytes; i++) {
        unsigned char b = 0;
        for (int j = 0; j < 8; j++) {
            b = (b << 1) | (bits[i * 8 + j] - '0');
        }
        outBuffer.push_back(static_cast<char>(b));
    }
    return outBuffer;
}

string codificarMetodo1_string(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    if (bloques == 0) return string();

    string out;
    out.resize(bloques * n);

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
        } else if (ceros > unos) {
            for (int i = 0; i < n; ++i)
                if (i % 2 == 1) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        } else {
            for (int i = 0; i < n; ++i)
                if (i % 3 == 2) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        }
    }
    return out;
}


// Decodificar Metodo 1
string decodificarMetodo1_aplicacion(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    if (bloques == 0) return string();

    string out;
    out.resize(bloques * n);

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
            (out[startPrev + i] == '1') ? ++unos : ++ceros;

        if (unos == ceros) {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
        } else if (ceros > unos) {
            for (int i = 0; i < n; ++i)
                if (i % 2 == 1) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        } else {
            for (int i = 0; i < n; ++i)
                if (i % 3 == 2) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        }
    }
    return out;
}

// Codificar Metodo 2
string codificarMetodo2_string(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int start = b * n;
        if (n == 1) { out[start] = bits[start]; continue; }
        out[start + 0] = bits[start + n - 1];
        for (int i = 1; i < n; ++i)
            out[start + i] = bits[start + i - 1];
    }
    return out;
}

// Decodificar Metodo 2
string decodificarMetodo2_aplicacion(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int start = b * n;
        if (n == 1) { out[start] = bits[start]; continue; }
        for (int i = 0; i < n - 1; ++i)
            out[start + i] = bits[start + i + 1];
        out[start + n - 1] = bits[start];
    }
    return out;
}

//sistema de archivos
string leerArchivoBinario(const string &ruta) {
    ifstream in(ruta, ios::binary);
    if (!in) throw runtime_error("No se pudo abrir archivo: " + ruta);
    in.seekg(0, ios::end);
    streamsize sz = in.tellg();
    in.seekg(0, ios::beg);
    string buffer((size_t)sz, '\0');
    in.read(&buffer[0], sz);
    return buffer;
}

void guardarArchivoBinario(const string &ruta, const string &data) {
    ofstream out(ruta, ios::binary);
    if (!out) throw runtime_error("No se pudo guardar archivo: " + ruta);
    out.write(data.data(), data.size());
}
