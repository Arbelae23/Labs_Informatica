#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
using namespace std;

// Convierte un byte en 8 caracteres 0 / 1 y los agrega a un string
void byteToBits(unsigned char byte, string &bits);

// Método 1 - Codificación por bloques de n bits
string encodeMethod1_fixed(const string &bits, int n);

// Método 2 - Codificación por rotación dentro de cada bloque
string encodeMethod2(const string &bits, int n);

#endif
