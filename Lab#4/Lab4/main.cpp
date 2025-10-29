#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <climits>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

// Clase Router
class Router {
public:
    Router(int id) : idRouter(id), distancia(INT_MAX), visitado(false), predecesor(nullptr) {}

    int idRouter;
    std::vector<std::pair<Router*, int>> vecinos;
    int distancia;
    bool visitado;
    Router* predecesor;

    void nuevoVecino(Router* vecino, int costo) {
        vecinos.emplace_back(vecino, costo);
    }

    void eliminarVecino(int idVecino) {
        vecinos.erase(std::remove_if(vecinos.begin(), vecinos.end(), [idVecino](const std::pair<Router*, int>& p) {
                          return p.first->idRouter == idVecino;
                      }), vecinos.end());
    }

    void confDistancia(int distancia) {
        this->distancia = distancia;
    }

    void reinicio() {
        distancia = INT_MAX;
        visitado = false;
        predecesor = nullptr;
    }

    void mostrarConexiones() {
        if (vecinos.empty()) {
            std::cout << "Router " << idRouter << " no tiene conexiones." << std::endl;
        } else {
            std::cout << "Router " << idRouter << " tiene las siguientes conexiones:" << std::endl;
            for (const auto& vecino : vecinos) {
                std::cout << " -> Router " << vecino.first->idRouter << " con un costo de " << vecino.second << std::endl;
            }
        }
    }
};

void dijkstra(Router* fuente) {
    fuente->confDistancia(0);

    std::priority_queue<std::pair<int, Router*>, std::vector<std::pair<int, Router*>>, std::greater<std::pair<int, Router*>>> pq;
    pq.push({0, fuente});

    while (!pq.empty()) {
        Router* actual = pq.top().second;
        pq.pop();

        if (actual->visitado) continue;

        actual->visitado = true;

        for (auto& vec : actual->vecinos) {
            Router* sigRouter = vec.first;
            int costoBorde = vec.second;

            int nuevaDistancia = actual->distancia + costoBorde;
            if (nuevaDistancia < sigRouter->distancia) {
                sigRouter->confDistancia(nuevaDistancia);
                sigRouter->predecesor = actual;
                pq.push({nuevaDistancia, sigRouter});
            }
        }
    }
}

void agregarRouter(std::unordered_map<int, Router*>& routers, int id) {
    if (routers.find(id) == routers.end()) {
        routers[id] = new Router(id);
        std::cout << "Router " << id << " agregado." << std::endl;
    } else {
        std::cout << "Router " << id << " ya existe." << std::endl;
    }
}

void eliminarRouter(std::unordered_map<int, Router*>& routers, int id) {
    auto it = routers.find(id);
    if (it != routers.end()) {
        for (auto it2 = routers.begin(); it2 != routers.end(); ++it2) {
            it2->second->eliminarVecino(id);
        }
        delete it->second;
        routers.erase(it);
        std::cout << "Router " << id << " eliminado." << std::endl;
    } else {
        std::cout << "Router " << id << " no existe." << std::endl;
    }
}

void conectarRouters(std::unordered_map<int, Router*>& routers, int id1, int id2, int costo) {
    if (routers.find(id1) != routers.end() && routers.find(id2) != routers.end()) {
        routers[id1]->nuevoVecino(routers[id2], costo);
        routers[id2]->nuevoVecino(routers[id1], costo);
        std::cout << "Router " << id1 << " conectado a Router " << id2 << " con costo " << costo << "." << std::endl;
    } else {
        std::cout << "Uno o ambos routers no existen." << std::endl;
    }
}

void resetRouters(std::unordered_map<int, Router*>& routers) {
    for (auto it = routers.begin(); it != routers.end(); ++it) {
        it->second->reinicio();
    }
}

void mostrarCamino(Router* destino) {
    if (destino->distancia == INT_MAX) {
        std::cout << "No hay camino desde el origen al destino." << std::endl;
        return;
    }

    std::stack<int> camino;
    Router* actual = destino;
    while (actual != nullptr) {
        camino.push(actual->idRouter);
        actual = actual->predecesor;
    }

    std::cout << "Camino mas corto: ";
    while (!camino.empty()) {
        std::cout << camino.top();
        camino.pop();
        if (!camino.empty()) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}

void calcularYMostrarCamino(std::unordered_map<int, Router*>& routers, int idFuente, int idDestino) {
    if (routers.find(idFuente) != routers.end() && routers.find(idDestino) != routers.end()) {
        resetRouters(routers);
        dijkstra(routers[idFuente]);
        mostrarCamino(routers[idDestino]);
    } else {
        std::cout << "Uno o ambos routers no existen." << std::endl;
    }
}

void borrarRedes(std::unordered_map<int, Router*>& routers) {
    for (auto it = routers.begin(); it != routers.end(); ++it) {
        delete it->second;
    }
    routers.clear();
    std::cout << "Todas las redes han sido borradas." << std::endl;
}

void generarRedesAleatorias(std::unordered_map<int, Router*>& routers) {
    borrarRedes(routers);

    std::srand(std::time(nullptr));

    for (int i = 0; i < 5; ++i) {
        agregarRouter(routers, i);
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int costo = std::rand() % 10 + 1;
            conectarRouters(routers, i, j, costo);
        }
    }
    std::cout << "Se han generado 5 redes aleatorias." << std::endl;
}

void leerArchivoYCrearRed(std::unordered_map<int, Router*>& routers) {
    // Borra todas las redes ya existentes
    borrarRedes(routers);

    const std::string nombreArchivo = "C:/Users/USER/Documents/GitHub/Labs_Informatica/Lab#4/Lab4/PRUEBAS.txt";
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string tipo;
        ss >> tipo;

        if (tipo == "Router") {
            int id;
            ss >> id;
            agregarRouter(routers, id);
        } else if (tipo == "Conectar") {
            int id1, id2, costo;
            ss >> id1 >> id2 >> costo;
            conectarRouters(routers, id1, id2, costo);
        } else {
            std::cerr << "Linea desconocida: " << linea << std::endl;
        }
    }

    archivo.close();
}

int main() {
    std::unordered_map<int, Router*> routers;

    // Ejemplo inicial de creación de routers y conexiones
    agregarRouter(routers, 0);
    agregarRouter(routers, 1);
    agregarRouter(routers, 2);
    agregarRouter(routers, 3);

    conectarRouters(routers, 0, 1, 2);
    conectarRouters(routers, 0, 2, 4);
    conectarRouters(routers, 1, 2, 1);
    conectarRouters(routers, 1, 3, 7);
    conectarRouters(routers, 2, 3, 3);

    std::string option;
    int id, id1, id2, costo;

    // Bucle del menú de opciones
    while (true) {
        std::cout << "\nMenu:\n1. Agregar Router\n2. Eliminar Router\n3. Conectar Routers\n4. Calcular distancia mas corta\n5. Mostrar camino\n6. Ver conexiones de un router\n7. Borrar y generar redes aleatorias\n8. Leer archivo y crear red\n9. Salir\n";
        std::cin >> option;

        if (option.length() != 1) {
            std::cout << "Opcion invalida." << std::endl;
            continue;
        }

        switch (option[0]) {
        case '1':
            std::cout << "Ingrese ID del nuevo Router: ";
            std::cin >> id;
            agregarRouter(routers, id);
            break;
        case '2':
            std::cout << "Ingrese ID del Router a eliminar: ";
            std::cin >> id;
            eliminarRouter(routers, id);
            break;
        case '3':
            std::cout << "Ingrese ID del Router de origen, ID del Router de destino y el costo: ";
            std::cin >> id1 >> id2 >> costo;
            conectarRouters(routers, id1, id2, costo);
            break;
        case '4':
            std::cout << "Ingrese ID del Router de origen e ID del Router de destino: ";
            std::cin >> id1 >> id2;
            calcularYMostrarCamino(routers, id1, id2);
            break;
        case '5':
            std::cout << "Ingrese ID del Router destino: ";
            std::cin >> id;
            mostrarCamino(routers[id]);
            break;
        case '6':
            std::cout << "Ingrese ID del Router para ver sus conexiones: ";
            std::cin >> id;
            if (routers.find(id) != routers.end()) {
                routers[id]->mostrarConexiones();
            } else {
                std::cout << "Router no encontrado." << std::endl;
            }
            break;
        case '7':
            generarRedesAleatorias(routers);
            break;
        case '8':
            leerArchivoYCrearRed(routers);
            break;
        case '9':
            borrarRedes(routers);
            return 0;
        default:
            std::cout << "Opcion invalida." << std::endl;
        }
    }

    return 0;
}
