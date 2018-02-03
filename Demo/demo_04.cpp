/*
 * @Author: bbdle 
 * @Date: 2018-02-03 23:10:22 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-03 23:14:43
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
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    int err;
    char ip[16] = "";
    unsigned int ipaddr;
    string message;

    // start socket library, for Windows
    StartSocketLib;

    // get ip
    cout << "Enter IP addresss to connect to: ";
    cin >> ip;

    // convert ip
    ipaddr = inet_addr(ip);
    if(ipaddr == -1)
    {
        cout << "Error: invalid IP address" << endl;
        return -1;
    }

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
    socketaddress.sin_addr.s_addr = ipaddr;
    // clear 8 bytes
    memset(&(socketaddress.sin_zero), 0, 8);

    // connect the socket
    err = connect(sock, (struct sockaddr*)&socketaddress, sa_size);
    
    // check error
    if (err == -1)
    {
        cout << "Socket connection error" << endl;
        return -1;
    }
    cout << "Socket connected!" << endl;

    bool done = false;
    cout << "Type data to send:" << endl;

    while(!done)
    {
        cin >> message;

        err = send(sock, message.c_str(), message.size() + 1, 0);
        
        if( err == -1 )
        {
            cout << "Socket sending error!" << endl;
            return 0;
        }

        if(message == "servquit" || message == "quit")
        {
            done = true;
        }
    }

    shutdown(sock, 2);
    CloseSocket(sock);

    CloseSocketLib;
}