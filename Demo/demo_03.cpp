/*
 * @Author: bbdle 
 * @Date: 2018-02-03 22:21:21 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-03 22:50:20
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
#include <vector>
using namespace std;

int main(int argc, char** argv)
{
    int err;
    int lsock;
    vector<int> socket_list;

    StartSocketLib;

    // create socket
    lsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // check error
    if(lsock == -1)
    {
        cout << "Socket creation error!" << endl;
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
    err = bind(lsock, (struct sockaddr*)&socketaddress, sa_size);

    // check error
    if (err == -1)
    {
        cout << "Socket binding error!" << endl;
        return -1;
    }
    cout << "Socket bound!" << endl;

    // listen on the socket
    err = listen(lsock, 16);

    // check error
    if (err == -1)
    {
        cout << "Socket listening error!" << endl;
        return 0;
    }
    cout << "Socket is listening, waiting for connection..." << endl;

    fd_set rset;
    int i;
    int max_socket = lsock;

    // create time struct
    struct timeval zerotime;
    zerotime.tv_usec = 0;
    zerotime.tv_sec = 0;

    char buffer[128];
    bool done = false;                      // server quit flag
    
    while(!done)
    {
        FD_ZERO(&rset);

        FD_SET(lsock, &rset);

        for(auto iter = socket_list.begin(); iter != socket_list.end(); ++iter)
        {
            FD_SET(*iter, &rset);
        }
        i = select(max_socket + 1, &rset, nullptr, nullptr, &zerotime);

        if(i > 0)
        {
            if(FD_ISSET(lsock, &rset))
            {
                // client comming
                cout << "We has a new client..." << endl;
                int clientsock = accept(lsock, (struct sockaddr*)&socketaddress, &sa_size);
                if(clientsock == -1)
                {
                    cout << "Socket accepting error!" << endl;
                    return -1;
                }
                cout << "Socket accepted" << endl;

                // add client to socket list
                socket_list.push_back(clientsock);
                if(clientsock > max_socket)
                {
                    max_socket = clientsock;
                }
            }

            for(auto iter = socket_list.begin(); iter != socket_list.end(); ++iter)
            {
                if(FD_ISSET(*iter, &rset))
                {
                    // Has data from client
                    cout << "read data from client!" << endl;
                    err = recv(*iter, buffer, 128, 0);

                    if(err == -1)
                    {
                        cout << "read data error" << endl;
                        return -1;
                    }
                    if(err == 0)
                    {
                        cout << "Socket Close" << endl;
                        shutdown(*iter, 2);
                        CloseSocket(*iter);
                        socket_list.erase(iter);
                        --iter;
                    }
                    else
                    {
                        cout << "Data:" << buffer << endl;
                        if(strncmp(buffer, "servquit", strlen(buffer)) == 0)
                        {
                            done = true;
                        }
                    }
                }
            }
        }
    }

    shutdown(lsock, 2);
    CloseSocket(lsock);

    for(auto iter = socket_list.begin(); iter != socket_list.end(); ++iter)
    {
        shutdown(*iter, 2);
        CloseSocket(*iter);
    }
    
    CloseSocketLib;
}