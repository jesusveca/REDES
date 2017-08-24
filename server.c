  /* Server code in C */
 
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
 
  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;
 
    if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    // pone la Ip del seridor automáticamente 
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    // int bind(int fd, struct sockaddr *my_addr,int addrlen);
    // fd Es el descriptor de fichero socket devuelto por la llamada a socket()
    // my_addr. es un puntero a una estructura sockaddr
    // addrlen. contiene la longitud de la estructura sockaddr a la cuál apunta el puntero my_addr. Se debería establecer como sizeof(struct sockaddr).

    // La llamada bind() se usa cuando los puertos locales de nuestra máquina 
    // están en nuestros planes (usualmente cuando utilizamos la llamada 
    // listen()). Su función esencial es asociar un socket con un puerto (de nuestra máquina). 
    // Análogamente socket(), devolverá -1 en caso de error.
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 

    // La función listen() se usa si se están esperando conexiones entrantes, 
    // lo cual significa, si se quiere, que alguien pueda conectarse a nuestra máquina.
    //Después de llamar a listen(), se deberá llamar a accept(), para así 
    // aceptar las conexiones entrantes. 
    // La secuencia resumida de llamadas al sistema es:
    // socket()
    // bind()
    // listen()
    // accept()

    if(-1 == listen(SocketFD, 10))
    {
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    for(;;)
    {
      // Cuando alguien intenta conectarse a nuestra computadora, se debe usar 
      // accept() para conseguir la conexión. Es muy fácil de entender: 
      //  alguien sólo podrá conectarse (asóciese con connect()) a nuestra máquina, 
      //  si nosotros aceptamos (asóciese con accept())
      int ConnectFD = accept(SocketFD, NULL, NULL);
      if(0 > ConnectFD)
      {
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }
 
 
     bzero(buffer,256); // fill the buffer 
     n = read(ConnectFD,buffer,255); // read the buffer
     if (n < 0) perror("ERROR reading from socket");
     printf("Here is the message: [%s]\n",buffer);
     n = write(ConnectFD,"I got your message",18); // write
     if (n < 0) perror("ERROR writing to socket");
 
     /* perform read write operations ... */
 
      shutdown(ConnectFD, SHUT_RDWR);
 
      close(ConnectFD);
    }
 
    close(SocketFD);
    return 0;
  }
