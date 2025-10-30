#include "redmanager.h"

RedManager::RedManager() {}

RedManager::~RedManager() {
    borrarRedes();
}

void RedManager::agregarRouter(int id) {
    if (routers.find(id) == routers.end()) {
        routers[id] = new Router(id);
        std::cout << "Router " << id << " agregado." << std::endl;
    } else {
        std::cout << "Router " << id << " ya existe." << std::endl;
    }
}


void RedManager::mostrarTodasLasConexiones() {
    if (routers.empty()) {
        std::cout << "No hay routers en la red actualmente." << std::endl;
        return;
    }

    std::cout << "\n========= LISTADO DE TODAS LAS CONEXIONES =========\n";
    for (auto it = routers.begin(); it != routers.end(); ++it) {
        Router* router = it->second;
        if (router) {
            router->mostrarConexiones();
            std::cout << "---------------------------------------------------\n";
        }
    }
}

void RedManager::eliminarRouter(int id) {
    auto it = routers.find(id);
    if (it != routers.end()) {
        for (auto& r : routers) {
            r.second->eliminarVecino(id);
        }
        delete it->second;
        routers.erase(it);
        std::cout << "Router " << id << " eliminado." << std::endl;
    } else {
        std::cout << "Router " << id << " no existe." << std::endl;
    }
}

void RedManager::conectarRouters(int id1, int id2, int costo) {
    if (routers.find(id1) != routers.end() && routers.find(id2) != routers.end()) {
        routers[id1]->nuevoVecino(routers[id2], costo);
        routers[id2]->nuevoVecino(routers[id1], costo);
        std::cout << "Router " << id1 << " conectado a Router " << id2
                  << " con costo " << costo << "." << std::endl;
    } else {
        std::cout << "Uno o ambos routers no existen." << std::endl;
    }
}

void RedManager::resetRouters() {
    for (auto& r : routers) {
        r.second->reinicio();
    }
}

void RedManager::dijkstra(Router* fuente) {
    fuente->confDistancia(0);
    std::priority_queue<std::pair<int, Router*>,
                        std::vector<std::pair<int, Router*>>,
                        std::greater<std::pair<int, Router*>>> pq;
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

void RedManager::mostrarCamino(Router* destino) {
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
        if (!camino.empty()) std::cout << " -> ";
    }
    std::cout << std::endl;

    std::cout << "Costo total: " << destino->distancia << std::endl;
}

void RedManager::calcularYMostrarCamino(int idFuente, int idDestino) {
    if (routers.find(idFuente) != routers.end() && routers.find(idDestino) != routers.end()) {
        resetRouters();
        dijkstra(routers[idFuente]);
        mostrarCamino(routers[idDestino]);
    } else {
        std::cout << "Uno o ambos routers no existen." << std::endl;
    }
}

void RedManager::borrarRedes() {
    for (auto& r : routers) {
        delete r.second;
    }
    routers.clear();
    std::cout << "Todas las redes han sido borradas." << std::endl;
}

void RedManager::generarRedesAleatorias() {
    borrarRedes();
    std::srand(std::time(nullptr));

    for (int i = 0; i < 5; ++i) agregarRouter(i);
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            int costo = std::rand() % 10 + 1;
            conectarRouters(i, j, costo);
        }
    }

    std::cout << "Se han generado 5 redes aleatorias.\n" << std::endl;

    std::cout << "Routers existentes y sus conexiones:" << std::endl;
    for (auto& r : routers) {
        r.second->mostrarConexiones();
    }
}

void RedManager::leerArchivoYCrearRed() {
    borrarRedes();

    const std::string nombreArchivo =
        "C:/Users/USER/Documents/GitHub/Labs_Informatica/Lab#4/Lab4/PRUEBAS.txt";

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
            agregarRouter(id);
        } else if (tipo == "Conectar") {
            int id1, id2, costo;
            ss >> id1 >> id2 >> costo;
            conectarRouters(id1, id2, costo);
        } else {
            std::cerr << "Linea desconocida: " << linea << std::endl;
        }
    }
    archivo.close();

    std::cout << "\nRed cargada desde archivo correctamente.\n" << std::endl;

    std::cout << "Routers existentes y sus conexiones:" << std::endl;
    for (auto& r : routers) {
        r.second->mostrarConexiones();
    }
}

void RedManager::mostrarConexionesDeRouter(int id) {
    if (routers.find(id) != routers.end()) {
        routers[id]->mostrarConexiones();
    } else {
        std::cout << "Router no encontrado." << std::endl;
    }
}
