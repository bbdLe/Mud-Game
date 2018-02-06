/*
 * @Author: bbdle 
 * @Date: 2018-02-06 19:27:38 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 19:32:26
 */

#ifndef __SOCKETLIBTYPES_H
#define __SOCKETLIBTYPES_H

#ifdef WIN32
    #include "winsock2.h"
    #include "Ws2tcpip.h"

    #ifndef socklen_t
        typedef int socklen_t;
    #endif
#else       // Linux or UNIX
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <errno.h>
    #include <fcntl.h>
#endif

namespace SocketLib
{
    #ifdef WIN32
        typedef SOCKET sock;
    #else
        typedef int sock;
    #endif

    typedef unsigned short int port;     // normal byte order
    typedef unsigned long int ipaddress; // network byte order
}

#endif