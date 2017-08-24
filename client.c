 /* Client code in C */
 
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
    int Res;

    // mayor info: 

    // Create the socket
    // 3 arguments : 1) Internet domain, Stream socket, Default protocol
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;
 
    // en caso de fallo retorna 1
    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
    // memset(pointerToTheBlockToFill, valueToBeSet, numberBitsToBeSet)
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    //es el tipo de conexión (por red o interna)
    stSockAddr.sin_family = AF_INET;
    // port number, 
    // function converts the unsigned short integer hostshort from host byte order to network byte order.
    // números se almacenan en la memoria en el orden de los bytes de la red, que es con el byte más significativo en primer lugar.
    stSockAddr.sin_port = htons(1100);
    //convert IPv4 and IPv6 addresses from text to binary form
    Res = inet_pton(AF_INET, "192.168.0.34", &stSockAddr.sin_addr);
    // returns 1 on success (network address was successfully
    // converted).  0 is returned if src does not contain a character string
    // representing a valid network address

    // inet_pton returns a negative value and sets errno to EAFNOSUPPORT if af
    // does not contain a valid address family.	 0 is returned if src does not
    // contain	a character string representing a valid network address in the
    // specified address family.  A positive value is returned if the  network
    // address was successfully converted.
    if (0 > Res) 
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    // admite los mismos parámetros, con la excepción de que el buffer de datos 
    // debe estar previamente relleno con la información que queremos enviar. 
    // write (Socket, message, longitud)
    n = write(SocketFD,"Hi, this is Julio.",18);
    
    /* perform read write operations ... */
    
    // buffer for receive the communication 
    char buffer[256];
    bzero(buffer,256); // set the buffer to 0

    n = read(SocketFD,buffer,255); // read

    // has not read any data yet, it returns -1 and sets errno to EINTR
    if (n < 0) perror("ERROR reading from socket");
    //has successfully read some data, it returns the number of bytes 
    // it read before it was interrupted
    printf("Server replay: [%s]\n",buffer);
    
    // turn off socket
    shutdown(SocketFD, SHUT_RDWR);

    // close socket
    close(SocketFD);
    return 0;
  }
