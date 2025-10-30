#ifndef REDMANAGER_H
#define REDMANAGER_H

#include "router.h"
#include <unordered_map>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

class RedManager {
private:
    std::unordered_map<int, Router*> routers;

public:
    RedManager();
    ~RedManager();

    void agregarRouter(int id);
    void eliminarRouter(int id);
    void conectarRouters(int id1, int id2, int costo);
    void resetRouters();
    void dijkstra(Router* fuente);
    void mostrarCamino(Router* destino);
    void calcularYMostrarCamino(int idFuente, int idDestino);
    void borrarRedes();
    void generarRedesAleatorias();
    void leerArchivoYCrearRed();
    void mostrarConexionesDeRouter(int id);
    void mostrarTodasLasConexiones();
};

#endif // REDMANAGER_H
