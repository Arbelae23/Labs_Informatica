#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "funciones.h"

using namespace std;

int main() {
    int opcion = 0;

    do {

        cout << "1. Codificar archivo (string)\n";
        cout << "2. Decodificar archivo (string)\n";
        cout << "3. Acceder como administrador\n";
        cout << "4. Acceder como usuario\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;



        cout << endl;

        switch (opcion) {
        case 1:{
            cout << "\n-- CODIFICACION CON STRING --\n";

            // Parte fija de la ruta
            string rutaBase = "C:/Users/USER/Documents/GitHub/Labs_Informatica/Lab#3/Lab3/";

            string nombreArchivo;
            string nombreSalida = rutaBase + "Codificado.bin"; // salida fija

            //  PEDIR ARCHIVO DE ENTRADA

            while (true) {
                cout << "Ingrese el nombre del archivo de entrada (ejemplo: Decodificado.txt): ";
                cin >> nombreArchivo;

                string rutaCompleta = rutaBase + nombreArchivo;

                ifstream test(rutaCompleta, ios::binary);
                if (test.good()) {
                    test.close();
                    nombreArchivo = rutaCompleta; // guardamos la ruta completa
                    break;
                } else {
                    cout << "No se pudo abrir el archivo en la ruta: " << rutaCompleta << "\n";
                }
            }

            //  PEDIR SEMILLA (n)

            int n;
            while (true) {
                cout << "Ingrese la semilla (entero positivo): ";
                cin >> n;

                if (!cin.fail() && n > 0) break;

                cout << "Valor invalido. Debe ser un entero positivo.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            //  PEDIR METODO (1 o 2)

            int metodo;
            while (true) {
                cout << "Ingrese metodo de codificacion (1 o 2): ";
                cin >> metodo;

                if (!cin.fail() && (metodo == 1 || metodo == 2)) break;

                cout << "Metodo invalido. Solo se permite 1 o 2.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            //  PROCESO DE CODIFICACION

            try {
                ifstream in(nombreArchivo, ios::binary);
                if (!in) throw runtime_error("No se pudo abrir el archivo de entrada");

                in.seekg(0, ios::end);
                streamsize sz = in.tellg();
                in.seekg(0, ios::beg);
                if (sz <= 0) throw runtime_error("El archivo esta vacio o invalido");

                string buffer(sz, '\0');
                in.read(&buffer[0], sz);
                in.close();

                // Convertir bytes a bits
                string bits;
                bits.reserve(buffer.size() * 8);
                for (unsigned char c : buffer)
                    byteToBits(c, bits);

                // Calcular bloques validos
                int bloques = bits.size() / n;
                if (bloques == 0) throw runtime_error("n es mayor que el numero de bits disponibles");
                string orig = bits.substr(0, bloques * n);

                cout << "\nBinario original:\n" << orig << "\n";

                // Aplicar metodo
                string cod;
                if (metodo == 1)
                    cod = encodeMethod1_fixed(orig, n);
                else
                    cod = encodeMethod2(orig, n);

                cout << "\nBinario codificado:\n" << cod << "\n";

                // Empaquetar bits a bytes
                int outBytes = cod.size() / 8;
                string outBuffer;
                outBuffer.reserve(outBytes);
                for (int i = 0; i < outBytes; ++i) {
                    unsigned char b = 0;
                    for (int j = 0; j < 8; ++j)
                        b = (b << 1) | (cod[i * 8 + j] - '0');
                    outBuffer.push_back(static_cast<char>(b));
                }

                // Guardar salida
                ofstream out(nombreSalida, ios::binary);
                if (!out) throw runtime_error("No se pudo crear el archivo de salida");
                out.write(outBuffer.data(), outBuffer.size());
                out.close();

                cout << "\nArchivo codificado guardado correctamente en: " << nombreSalida << endl;
            }
            catch (const exception &e) {
                cerr << "Error: " << e.what() << endl;
                return 1;
            }

            break;
        }

        case 2:
            cout << "[Opcion seleccionada] Decodificar archivo (string)\n";
            // Aquí se llamará a la función de decodificación
            break;

        case 3:
            cout << "[Opcion seleccionada] Acceder como administrador\n";
            // Aquí se llamará a la función para validar y registrar usuarios
            break;

        case 4:
            cout << "[Opcion seleccionada] Acceder como usuario\n";
            // Aquí se llamará a la función para consultar o retirar dinero
            break;

        case 5:
            cout << "Saliendo del programa...\n";
            break;

        default:
            cout << "Opcion invalida. Intente de nuevo.\n";
            break;
        }

    } while (opcion != 5);

    return 0;
}
