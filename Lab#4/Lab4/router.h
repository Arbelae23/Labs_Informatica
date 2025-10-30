#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <vector>
#include <utility>
#include <climits>
#include <algorithm>

class Router {
public:
    int idRouter;
    std::vector<std::pair<Router*, int>> vecinos;
    int distancia;
    bool visitado;
    Router* predecesor;

    Router(int id);
    void nuevoVecino(Router* vecino, int costo);
    void eliminarVecino(int idVecino);
    void confDistancia(int distancia);
    void reinicio();
    void mostrarConexiones();
};

#endif // ROUTER_H
