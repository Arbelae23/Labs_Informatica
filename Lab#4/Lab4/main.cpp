#include "redmanager.h"

int main() {
    RedManager red;
    std::string option;
    int id, id1, id2, costo;

    std::cout << "============================================" << std::endl;
    std::cout << "        SIMULADOR DE REDES - UDEA" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "\nRed inicial tipo arbol + anillo + conexiones extra (8 routers):\n" << std::endl;

    // Crear routers iniciales
    for (int i = 0; i <= 7; ++i) {
        red.agregarRouter(i);
    }

    // Conexiones tipo árbol
    red.conectarRouters(0, 1, 2);
    red.conectarRouters(0, 2, 4);
    red.conectarRouters(1, 3, 3);
    red.conectarRouters(1, 4, 5);

    // Conexión tipo anillo
    red.conectarRouters(4, 0, 6);

    // Conexiones adicionales con router 2
    red.conectarRouters(2, 5, 2);
    red.conectarRouters(2, 6, 3);

    // Router 7 conectado a 3, 4, 5, 6
    red.conectarRouters(7, 3, 4);
    red.conectarRouters(7, 4, 4);
    red.conectarRouters(7, 5, 5);
    red.conectarRouters(7, 6, 5);

    // Mostrar red inicial (routers y conexiones reales)
    //std::cout << "\nRouters existentes y sus conexiones:\n" << std::endl;
    red.mostrarTodasLasConexiones();

    while (true) {
        std::cout << "\nMenu:\n"
                  << "1. Agregar Router\n"
                  << "2. Eliminar Router\n"
                  << "3. Conectar Routers\n"
                  << "4. Calcular distancia mas corta (muestra el camino y costo)\n"
                  << "5. Ver conexiones de un router\n"
                  << "6. Borrar y generar redes aleatorias\n"
                  << "7. Leer archivo y crear red\n"
                  << "8. Salir\n";
        std::cin >> option;

        if (option.length() != 1) {
            std::cout << "Opcion invalida." << std::endl;
            continue;
        }

        switch (option[0]) {
        case '1':
            std::cout << "Ingrese ID del nuevo Router: ";
            std::cin >> id;
            red.agregarRouter(id);
            break;
        case '2':
            std::cout << "Ingrese ID del Router a eliminar: ";
            std::cin >> id;
            red.eliminarRouter(id);
            break;
        case '3':
            std::cout << "Ingrese ID del Router de origen, ID del Router de destino y el costo: ";
            std::cin >> id1 >> id2 >> costo;
            red.conectarRouters(id1, id2, costo);
            break;
        case '4':
            std::cout << "Ingrese ID del Router de origen e ID del Router de destino: ";
            std::cin >> id1 >> id2;
            red.calcularYMostrarCamino(id1, id2);
            break;
        case '5':
            std::cout << "Ingrese ID del Router para ver sus conexiones: ";
            std::cin >> id;
            red.mostrarConexionesDeRouter(id);
            break;
        case '6':
            red.borrarRedes();
            red.generarRedesAleatorias();
            break;
        case '7':
            red.leerArchivoYCrearRed();
            red.mostrarTodasLasConexiones();
            break;
        case '8':
            red.borrarRedes();
            return 0;
        default:
            std::cout << "Opcion invalida." << std::endl;
        }
    }
}
