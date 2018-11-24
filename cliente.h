#ifndef CLIENTE_H
#define CLIENTE_H
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <netdb.h>
#include "definitions.h"
#include "JSON/json.hpp"
// #include <vlc/vlc.h>

class Cliente
{
private:
    bool salir;
    int client_socket = -1;
    struct sockaddr_in server_addr;
    socklen_t size;

    std::string DireccionIP = DIRECCION_IP;

    using json =  nlohmann::json;

public:

    Cliente(sockaddr_in server_addr);
    ~Cliente();
    void iniciar();
    void ejecutar();
    void detener();

    bool conectar();
    bool enviarDatos(std::string data);
    std::string recibeDatos(int size);
    std::string  recibirVideo();

    void subirVideo(std::string path);
    void enviarSolicitud(int numSolicitud);
//    std::string metadata(std::string path);


};

#endif // CLIENTE_H
