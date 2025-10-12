#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <limits>
#include "funciones.h"

using namespace std;

int main() {
    {
        string cedula = "1052837312";
        string clave = "321";
        int n = 3, metodo = 1;

        string bits;
        for (unsigned char c : clave)
            convertirByteABits_aplicacion(c, bits);

        string cod;
        if (metodo == 1)
            cod = codificarMetodo1_string(bits, n);
        else
            cod = codificarMetodo2_string(bits, n);

        string claveCod = convertirBitsABytes_aplicacion(cod);

        string ruta = "C:/Users/USER/Documents/GitHub/Labs_Informatica/Lab#3/Lab3/sudo.txt";
        ofstream out(ruta, ios::binary);
        out << cedula << "," << claveCod << "," << n << "," << metodo << "\n";
        out.close();

        cout << "Archivo sudo.txt generado correctamente.\n";
    }
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
            cout << "\n-- DECODIFICACION CON STRING --\n";

            try {
                // RUTA BASE (misma que en el caso 1)
                string rutaBase = "C:/Users/USER/Documents/GitHub/Labs_Informatica/Lab#3/Lab3/";
                string rutaEntrada = rutaBase + "Codificado.bin";
                string rutaSalida = rutaBase + "Decodificado_resultado.txt"; // nombre diferente para no sobreescribir

                // Pedir semilla (n)
                int n;
                while (true) {
                    cout << "Ingrese la semilla (entero positivo): ";
                    cin >> n;
                    if (!cin.fail() && n > 0) break;
                    cout << "Valor invalido. Debe ser un entero positivo.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }

                // Pedir metodo
                int metodo;
                while (true) {
                    cout << "Ingrese metodo (1 o 2): ";
                    cin >> metodo;
                    if (!cin.fail() && (metodo == 1 || metodo == 2)) break;
                    cout << "Metodo invalido. Solo se permite 1 o 2.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }

                // Leer archivo codificado
                ifstream in(rutaEntrada, ios::binary);
                if (!in) throw runtime_error("No se pudo abrir el archivo Codificado.bin.");

                in.seekg(0, ios::end);
                streamsize sz = in.tellg();
                in.seekg(0, ios::beg);
                if (sz <= 0) throw runtime_error("El archivo esta vacio o es invalido.");

                string buffer((size_t)sz, '\0');
                in.read(&buffer[0], sz);
                in.close();

                // Convertir bytes a bits
                string bits;
                bits.reserve(buffer.size() * 8);
                for (unsigned char c : buffer)
                    byteToBits_dec(c, bits);

                // Decodificar segun metodo
                string decoded;
                if (metodo == 1)
                    decoded = decodeMethod1(bits, n);
                else
                    decoded = decodeMethod2(bits, n);

                cout << "\nBits decodificados:\n" << decoded << "\n";

                // Convertir bits a bytes (texto original)
                string outBuffer;
                int outBytes = decoded.size() / 8;
                outBuffer.reserve(outBytes);
                for (int i = 0; i < outBytes; ++i)
                    outBuffer += static_cast<char>(bitsToByte_dec(decoded, i * 8));

                // Guardar resultado en archivo de texto
                ofstream out(rutaSalida, ios::binary);
                if (!out) throw runtime_error("No se pudo crear el archivo de salida.");
                out.write(outBuffer.data(), outBuffer.size());
                out.close();

                cout << "\nArchivo decodificado guardado correctamente en:\n" << rutaSalida << endl;
                cout << "Contenido decodificado:\n" << outBuffer << "\n";

            } catch (const exception &e) {
                cerr << "Error: " << e.what() << endl;
                return 1;
            }

            break;

        case 3:{
            cout << "\n-- MODO ADMINISTRADOR --\n";
            try {
                // Ruta base
                string rutaBase = "C:/Users/USER/Documents/GitHub/Labs_Informatica/Lab#3/Lab3/";
                string rutaSudo = rutaBase + "sudo.txt";

                ifstream in(rutaSudo, ios::binary);
                if (!in.is_open()) throw runtime_error("No se encontro " + rutaSudo);

                // Pedir credenciales
                cin.ignore();
                cout << "Ingrese cedula de admin: ";
                string cedulaAdmin; getline(cin, cedulaAdmin);
                cout << "Ingrese clave: ";
                string clave; getline(cin, clave);

                if (cedulaAdmin.empty() || clave.empty())
                    throw runtime_error("Debe ingresar cedula y clave.");

                bool valido = false;
                int nAdmin = 0, metodoAdmin = 0;
                string linea;

                while (getline(in, linea)) {
                    stringstream ss(linea);
                    string cedulaF, claveF, nF, metodoF;
                    getline(ss, cedulaF, ',');
                    getline(ss, claveF, ',');
                    getline(ss, nF, ',');
                    getline(ss, metodoF, ',');

                    if (cedulaF == cedulaAdmin) {
                        nAdmin = stoi(nF);
                        metodoAdmin = stoi(metodoF);

                        string bits;
                        for (unsigned char c : claveF)
                            convertirByteABits_aplicacion(c, bits);

                        string claveDec;
                        if (metodoAdmin == 1)
                            claveDec = decodificarMetodo1_aplicacion(bits, nAdmin);
                        else
                            claveDec = decodificarMetodo2_aplicacion(bits, nAdmin);

                        string claveReal = convertirBitsABytes_aplicacion(claveDec);
                        cout << "[DEBUG] Clave decodificada: " << claveReal << endl;

                        if (claveReal == clave) valido = true;
                        break;
                    }
                }
                in.close();

                if (!valido)
                    throw runtime_error("Admin no autorizado o clave incorrecta.");

                cout << "\nAcceso como ADMINISTRADOR exitoso.\n";

                // Registro de usuario nuevo
                cout << "Desea registrar un nuevo usuario? (s/n): ";
                char op; cin >> op; cin.ignore();

                if (op == 's' || op == 'S') {
                    string cedula, claveU, saldo;
                    cout << "Ingrese cedula del usuario: ";
                    getline(cin, cedula);
                    cout << "Ingrese clave: ";
                    getline(cin, claveU);
                    cout << "Ingrese saldo inicial: ";
                    getline(cin, saldo);

                    if (cedula.empty() || claveU.empty() || saldo.empty())
                        throw runtime_error("Campos vacíos al registrar usuario.");

                    int nUser, metodoUser;
                    cout << "Ingrese semilla (n) para el usuario (1-7): ";
                    cin >> nUser; cin.ignore();
                    cout << "Ingrese metodo (1 o 2): ";
                    cin >> metodoUser; cin.ignore();

                    if (nUser < 1 || nUser > 7)
                        throw runtime_error("Semilla fuera de rango (1-7).");
                    if (metodoUser != 1 && metodoUser != 2)
                        throw runtime_error("Método invalido (solo 1 o 2).");

                    string bits;
                    for (unsigned char c : claveU)
                        convertirByteABits_aplicacion(c, bits);

                    string cod;
                    if (metodoUser == 1)
                        cod = codificarMetodo1_string(bits, nUser);
                    else
                        cod = codificarMetodo2_string(bits, nUser);

                    string claveCod = convertirBitsABytes_aplicacion(cod);

                    string rutaUsuarios = rutaBase + "usuarios.txt";
                    ofstream out(rutaUsuarios, ios::app);
                    if (!out.is_open())
                        throw runtime_error("No se pudo abrir usuarios.txt para escribir.");
                    out << cedula << "," << claveCod << "," << saldo
                        << "," << nUser << "," << metodoUser << "\n";
                    out.close();

                    cout << "Usuario registrado correctamente.\n";
                }

            } catch (const exception &e) {
                cerr << "Error en modo ADMIN: " << e.what() << "\n";
            }
            break;
        }

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
