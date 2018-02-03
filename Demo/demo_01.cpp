/*
 * @Author: bbdle 
 * @Date: 2018-02-03 20:57:42 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-03 21:25:17
 */

// Sockets API
#ifdef WIN32                    // Windows
    #include "win32sock2.h"
    #include "Ws2tcpip.h"

#else                           // UNIX or Linux
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <netdb.h>
    #include <arpa/inet.h>
#endif

#include <memory.h>

// For Compatibility
#ifdef WIN32
    WSADATA g_wsadata;
    #define CloseSocket closesocket
    #define GetSocketError WSAGetLastError
    #define StartSocketLib WSAStartup(MAKEWORD(2, 2), &g_wsadata);
    #define CloseSocketLib WSACleanup();

    #ifndef socklen_t
        typedef int socklen_t;
    #endif
#else
    #define CloseSocket close
    #define GetSocketError errno
    #define StartSocketLib {}
    #define CloseSocketLib {}
#endif

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    int err;

    // start socket library, for Windows
    StartSocketLib;

    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // check error
    if (sock == -1)
    {
        cout << "Socket create error!" << endl;
        return -1;
    }
    cout << "Socket created!" << endl;

    // create sockaddr_in struct
    struct sockaddr_in socketaddress;
    socklen_t sa_size = sizeof(struct sockaddr_in);
    socketaddress.sin_family = AF_INET;
    socketaddress.sin_port = htons(5555);
    socketaddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // clear 8 bytes
    memset(&(socketaddress.sin_zero), 0, 8);

    // bind socket
    err = bind(sock, (struct sockaddr*)&socketaddress, sa_size);

    // check error
    if (sock == -1)
    {
        cout << "Socket binding error!" << endl;
        return -1;
    }
    cout << "Socket bound!" << endl;

    // listen on the socket
    err = listen(sock, 16);

    // check error
    if (sock == -1)
    {
        cout << "Socket listening error!" << endl;
        return 0;
    }
    cout << "Socket is listening, waiting for connection..." << endl;

    // wait for client
    int clientsock;
    clientsock = accept(sock, (struct sockaddr*)&socketaddress, &sa_size);

    if (clientsock == -1)
    {
        cout << "Socket accepting error!" << endl;
        return -1;
    }
    cout << "Socket accepted, waiting for data..." << endl;

    // read data
    char temp_buffer[128];
    err = recv(clientsock, temp_buffer, 128, 0);

    // check error
    if(err == -1)
    {
        cout << "Socket read error" << endl;
        return -1;
    }
    cout << "Read data:";
    cout << temp_buffer << endl;

    // close client socket
    shutdown(clientsock, 2);
    CloseSocket(clientsock);

    // close server socket
    shutdown(sock, 2);
    CloseSocket(sock);

    // for windows
    CloseSocketLib;
}