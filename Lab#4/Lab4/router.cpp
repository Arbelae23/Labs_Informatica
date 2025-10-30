#include "router.h"

Router::Router(int id)
    : idRouter(id), distancia(INT_MAX), visitado(false), predecesor(nullptr) {}

void Router::nuevoVecino(Router* vecino, int costo) {
    vecinos.emplace_back(vecino, costo);
}

void Router::eliminarVecino(int idVecino) {
    vecinos.erase(std::remove_if(vecinos.begin(), vecinos.end(),
                                 [idVecino](const std::pair<Router*, int>& p) {
                                     return p.first->idRouter == idVecino;
                                 }), vecinos.end());
}

void Router::confDistancia(int distancia) {
    this->distancia = distancia;
}

void Router::reinicio() {
    distancia = INT_MAX;
    visitado = false;
    predecesor = nullptr;
}

void Router::mostrarConexiones() {
    if (vecinos.empty()) {
        std::cout << "Router " << idRouter << " no tiene conexiones." << std::endl;
    } else {
        std::cout << "Router " << idRouter << " tiene las siguientes conexiones:" << std::endl;
        for (const auto& vecino : vecinos) {
            std::cout << " -> Router " << vecino.first->idRouter
                      << " con un costo de " << vecino.second << std::endl;
        }
    }
}

