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

std::string  Cliente::recibirVideo() {
    enviarSolicitud(2);
    std::cout << recibeDatos(30) << std::endl; //Mensaje Recibido
    //Finaliza Json con la solicitud uno y entra al loop de enviar video

    std::string path = "/Users/juanpablomartinezbrenes/Desktop/OdisseyMediaPlayer/videos/prueba1.mp4";
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
    } else printf("Error, no se pudo abrir el archivo [%s] para recibir los datos.\n", "prueba.webm");

    std::cout << "Video Recibido"<< std::endl;

    return path;
}

void Cliente::enviarSolicitud(int numSolicitud){
    //Json con la solicitud uno que es subir un video
    json solicitud;
    solicitud["Solicitud"] = numSolicitud;
    std::string stringEnviar = solicitud.dump();
    char *mensajejsonchar = &stringEnviar[0u];
    enviarDatos(mensajejsonchar); //Envia Json con solicitud
}

void Cliente::subirVideo(std::string path){

    enviarSolicitud(1);
    std::cout << recibeDatos(30) << std::endl; //Mensaje Recibido
    //Finaliza Json con la solicitud uno y entra al loop de enviar video

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
//std::string Cliente::metadata(std::string path){

//    libvlc_instance_t* media = libvlc_new(0,NULL);
//    if(media == NULL){
//        perror("Error");
//        exit(1);
//    }
//    libvlc_media_t *meta;

//    meta = libvlc_media_new_path(media, path.c_str());

//    json metadata;

//    metadata["Titulo: "] = libvlc_media_get_meta(meta,libvlc_meta_Title);
//    //metadata["Artista: "] = libvlc_media_get_meta(meta,libvlc_meta_Artist);
//    //metadata["Album: "] = libvlc_media_get_meta(meta,libvlc_meta_Album);
//    //metadata["año"] = libvlc_media_get_meta(meta,libvlc_meta_Date);
//    //metadata["Contenedor: "] = libvlc_media_get_meta(meta,libvlc_meta_Description);

//    //Convierte a char el Json con la metadata
//    std::string stringEnviar = metadata.dump();
//    char *mensajejsonchar = &stringEnviar[0u];

//    return mensajejsonchar;
//}
