#include "cliente.h"
typedef unsigned short uint16;
static const int BUFFER_SIZE = 16*1024;

Cliente::Cliente(sockaddr_in server_addr){
    salir = false;
}

void Cliente::detener(){
    enviarSolicitud(99);
    salir = true;
    shutdown(client_socket,SHUT_RDWR);
}

void Cliente::iniciar(){
    client_socket = socket(AF_INET,SOCK_STREAM,0);
    conectar();
}

bool Cliente::conectar(){
    if(client_socket == -1){
        perror("No se pudo conectar el socket");

    }
    std::cout << "Socket creado \n" << std::endl;

    if( inet_addr(DireccionIP.c_str()) == -1){
        struct hostent *he;
        struct in_addr **addr_list;

        if( (he = gethostbyname(DireccionIP.c_str()) ) == NULL){
            herror("gethostbyname");
            std::cout << "No se pudo arreglar el HostName\n";
            return false;
        }

        addr_list = ( struct in_addr ** ) he->h_addr_list;
        for(int i = 0; addr_list[i] != NULL; i++ ){
            server_addr.sin_addr = *addr_list[i];
            std::cout << DireccionIP<< "Resolved to " << inet_ntoa(*addr_list[i])<<std::endl;
            break;
        }
    }
    else{
        server_addr.sin_addr.s_addr = inet_addr( DireccionIP.c_str() );
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( PORT );

    if ( connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ){
        perror( "Error: conexion fallida" );
        return 1;
    }

    std::string mensajeConfirmacion = "->Copiado";
    enviarDatos(mensajeConfirmacion);
    std::cout << recibeDatos(50) << std::endl;
    std::cout << "Conexión Realizada\n"<< std::endl;

    return true;
}

bool Cliente::enviarDatos(std::string data){

    if(send(client_socket,data.c_str(),strlen(data.c_str() ),0) < 0){
        perror("Error envio de datos fallidos");
        return false;
    }
    std::cout << "Datos enviados: "<< data << std::endl;
    return true;
}

std::string Cliente::recibeDatos(int size){


    char buffer[size];
    std::string reply;

    if( recv( client_socket, buffer, size, 0 ) < 0 ){
        //puts("Entrega Fallida");
        std::cout << "Entrega Fallida" << std::endl;
    }
    reply = buffer;
    return reply;
}

void Cliente::enviarSolicitud(int numSolicitud){
    //Json con la solicitud uno que es subir un video
    json solicitud;
    solicitud["Solicitud"] = numSolicitud;
    std::string stringEnviar = solicitud.dump();
    char *mensajejsonchar = &stringEnviar[0u];
    enviarDatos(mensajejsonchar); //Envia Json con solicitud
}

void Cliente::guardarVideo(std::string path){

    enviarSolicitud(1);
    std::cout << recibeDatos(30) << std::endl; //Mensaje Recibido
    //Finaliza Json con la solicitud uno y entra al loop de enviar video

    enviarDatos(metadata(path));

    FILE * fpIn = fopen(path.c_str(), "r");
    if (fpIn)
    {
       char buf[BUFFER_SIZE];
       std::string byte = "";
       while(1)
       {
           ssize_t bytesRead = fread(buf, 1, sizeof(buf), fpIn);

           byte = std::to_string((int)bytesRead);
           send(client_socket, byte.c_str(),5, 0);
           //std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
           //enviarDatos(byte);
           std::cout << byte << std::endl;

           if (bytesRead <= 0){
               std::cout << "Video Enviado total"<< std::endl;
               break;  // EOF
           }
           printf("Read %i bytes from file, sending them to network...\n", (int)bytesRead);
           if (send(client_socket, buf, bytesRead, 0) != bytesRead)
           {
              perror("send");
              break;
           }

        }

       fclose(fpIn);
    }
    else printf("Error, couldn't open file [%s] to receive!\n", "fileName");
    std::cout << "Video Enviado"<< std::endl;
}
std::string Cliente::metadata(std::string path){
    json metadata;
    std::string titulo = path.substr(39, path.length() - 4);

    metadata["Titulo: "] = titulo;
    metadata["Artista: "] = "Desconocido";
    metadata["Album: "] = "Desconocido";
    metadata["Año: "] = "Desconocido";
    metadata["Duracion: "] = "Desconocido";
    metadata["Contenedor: "] = "Desconocido";

    //Convierte a char el Json con la metadata
    std::string stringEnviar = metadata.dump();

    std::cout << stringEnviar << std::endl;
    char *mensajejsonchar = &stringEnviar[0u];

    return mensajejsonchar;
}

std::string Cliente::reproducirVideo() {

    enviarSolicitud(2);
    std::cout <<recibeDatos(30) << std::endl; //Mensaje Recibido
    //Finaliza Json con la solicitud uno y entra al loop de enviar video

    std::string path = "/Users/juanpablomartinezbrenes/Desktop/prueba5.mp4";

    FILE * fpIn = fopen(path.c_str(), "w");
    if (fpIn)
    {
        char buf[BUFFER_SIZE];
        char* buffer = (char *) calloc(1, 5);
        int sizeOfBuffer = 0;
        ssize_t rec = 0;

        while(1)
        {
            recv(client_socket, buffer, 5, 0);
            sizeOfBuffer = std::stoi(buffer);
            if( sizeOfBuffer  <= 0 ){
                break;
            }
            rec = 0;
            do {
                ssize_t bytesReceived = recv(client_socket, buf, sizeOfBuffer - rec , 0);

                if (bytesReceived < 0) {
                    perror("recv");  // network error?
                    break;
                }
                if (bytesReceived == 0) {
                    std::cout << "Se Desconecto el Cliente" << std::endl;
                    break;
                }   // sender closed connection, must be end of file
                printf(" Cant de bytes recibidos desde el cliente: %i...Escribiendo en el archivo...\n",
                       (int) bytesReceived);
                if (fwrite(buf, 1, bytesReceived, fpIn) != (size_t) bytesReceived) {
                    perror("fwrite");
                    break;
                }
                memset(buf, 0, BUFFER_SIZE);
                rec += (int) bytesReceived;
            }while( rec < sizeOfBuffer );
        }

        fclose(fpIn);
        memset(buf,0,BUFFER_SIZE);
        free(buffer);

    } else{
        std::cout << "Error, no se pudo abrir el archivo " << path << std::endl;
    }

    std::cout << "Video Recibido"<< std::endl;

    return path;
}

void Cliente::eliminarVideo(){
    enviarSolicitud(3);
    std::cout << recibeDatos(256) << std::endl;
}

bool Cliente::buscarVideo(std::string nombre){

    enviarSolicitud(3);
    std::cout << recibeDatos(30) << std::endl;

    enviarDatos(nombre);

    std::string respuesta = recibeDatos(14);
    std::cout << respuesta << std::endl;

    respuesta = recibeDatos(15);
    std::cout << "respesuta: " << respuesta << std::endl;

    if(respuesta == "encontrado"){
        return true;
    }else{
        return false;
    }
}

std::string Cliente::cambiarMetadata(std::string nombre){
    enviarSolicitud(5);
    std::cout << recibeDatos(30) << std::endl;

    enviarDatos(nombre);
    std::string respuesta = recibeDatos(14);
    std::cout << respuesta << std::endl;

    std::string d = recibeDatos(3);
    std::cout<< d << std::endl;


    if (d != "999"){
        respuesta = recibeDatos(std::stoi(d));
        std::string respuestaNueva = respuesta.substr(0, respuesta.size());
        std::cout << respuestaNueva << std::endl;

        return respuestaNueva;
    }else{
        return "Noencontrado";
    }

}

bool Cliente::eliminarVideo(std::string nombre){
    enviarSolicitud(4);
    std::cout << recibeDatos(30) << std::endl;

    enviarDatos(nombre);
    std::string d = recibeDatos(1);
    std::cout << d << std::endl;

    std::string respuesta = d.substr(0, 1);
    std::cout << respuesta << std::endl;

    if(respuesta != "1"){
        std::cout << "Se eliminó el video: " << nombre << std::endl;
        return true;
    }else{
        std::cout << "No se pudo eliminar el video: " << nombre << std::endl;
        return false;
    }
}
