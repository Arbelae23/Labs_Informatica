#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
using namespace std;

// FUNCIONES DE CODIFICADOR

// Convierte un byte en 8 caracteres 0 / 1 y los agrega a un string
void byteToBits(unsigned char byte, string &bits);

// Método 1 - Codificación por bloques de n bits
string encodeMethod1_fixed(const string &bits, int n);

// Método 2 - Codificación por rotación dentro de cada bloque
string encodeMethod2(const string &bits, int n);

//FUNCIONES DE  DECODIFICADOR

// Convierte un byte en su representación binaria (8 bits) y los agrega a un string
void byteToBits_dec(unsigned char byte, string &bits);

// Convierte una secuencia de bits (a partir de una posición) en un byte
unsigned char bitsToByte_dec(const string &bits, int start);

// Método 1 - Decodificación de bloques de n bits (tamaño fijo)
string decodeMethod1(const string &bits, int n);

// Método 2 - Decodificación por rotación dentro de cada bloque
string decodeMethod2(const string &bits, int n);

// Convierte una cadena de bits a su representación en bytes (string)
string convertirBitsABytes_string(const string &bits);

#endif
