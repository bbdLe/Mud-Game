/*
 * @Author: bbdle 
 * @Date: 2018-02-06 20:13:39 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 22:01:01
 */

#include "SocketLibSocket.h"
#include <cstring>

namespace SocketLib
{
    Socket::Socket(sock p_sock) : m_sock(p_sock), m_isblocking(true)
    {
        if(p_sock != -1)
        {
            socklen_t s = sizeof(m_localinfo);
            getsockname(m_sock, reinterpret_cast<sockaddr*>(&m_localinfo), &s);
        }
    }

    void Socket::Close()
    {
        #ifdef WIN32
            closesocket(m_sock);
        #else
            close(m_sock);
        #endif

        m_sock = -1;
    }

    void Socket::SetBlocking(bool p_blockmode)
    {
        int err;

        #ifdef WIN32
            unsigned long mode = !p_blockmode;
            err = ioctlsocket(m_sock, FIONBIO, &mode);
        #else
            int flags = fcntl(m_sock, F_GETFL, 0);

            if(!p_blockmode)
            {
                flags |= O_NONBLOCK;
            }
            else
            {
                flags &= ~O_NONBLOCK;
            }
            err = fcntl(m_sock, F_SETFL, flags);
        #endif

        if( err == -1 )
        {
            throw(Exception(GetError()));
        }

        m_isblocking = p_blockmode;
    }

    void Socket::ResetInfo()
    {
        socklen_t s = sizeof(m_localinfo);
        int err = getsockname(m_sock, reinterpret_cast<sockaddr*>(&m_localinfo), &s);

        if(err != 0)
        {
            throw Exception(GetError());
        }
    }

    // ====================================================================
    // Client Socket
    // ====================================================================
    DataSocket::DataSocket(sock p_sock)
    : Socket(p_sock),
      m_connected(false)
    {
        if(p_sock != -1)
        {
            socklen_t s = sizeof(m_remoteinfo);
            getpeername(p_sock, reinterpret_cast<sockaddr*>(&m_remoteinfo), &s);
            m_connected = true;
        }
    }

    void DataSocket::Connect(ipaddress p_addr, port p_port)
    {
        int err;

        if(m_connected == true)
        {
            throw Exception(EAlreadyConnected);
        }

        // socket not exist
        if(m_sock == -1)
        {
            m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if(m_sock == -1)
            {
                throw Exception(GetError());
            }
        }

        // normal socket setup
        m_remoteinfo.sin_family = AF_INET;
        m_remoteinfo.sin_addr.s_addr = p_addr;
        m_remoteinfo.sin_port = htons(p_port);
        memset(&(m_remoteinfo.sin_zero), 0, 8);

        // connect
        socklen_t s = sizeof(struct sockaddr);
        err = connect(m_sock, reinterpret_cast<struct sockaddr*>(&m_remoteinfo), s);
        if(err == -1)
        {
            throw Exception(GetError());
        }
        
        m_connected = true;

        ResetInfo();
    }

    int DataSocket::Send(const char* p_buffer, int p_size)
    {
        if(!m_connected)
        {
            throw Exception(ENotConnected);
        }

        int err = send(m_sock, p_buffer, p_size, 0);
        if(err == -1)
        {
            Error e = GetError();
            if(e != EOperationWouldBlock)
            {
                throw Exception(e);
            }

            err = 0;
        }

        return err;
    }

    int DataSocket::Receive(char* p_buffer, int p_size)
    {
        if(!m_connected)
        {
            throw Exception(ENotConnected);
        }

        int err = recv(m_sock, p_buffer, p_size, 0);
        
        if(err == 0)
        {
            throw Exception(EConnectionClosed);
        }
        
        if(err == -1)
        {
            throw Exception(GetError());
        }

        return err;
    }

    void DataSocket::Close()
    {
        if(m_connected == true)
        {
            shutdown(m_sock, 2);
        }

        Socket::Close();
        m_connected = false;
    }

    // ====================================================================
    // Server Socket
    // ====================================================================
    ListeningSocket::ListeningSocket()
    {
        m_listening = false;
    }

    void ListeningSocket::Listen(port p_port)
    {
        if(m_sock == -1)
        {
            m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if(m_sock == -1)
            {
                throw Exception(GetError());
            }
        }

        int reuse = 1;
        int err = setsockopt(m_sock, SOL_SOCKET,
                             SO_REUSEADDR, &reuse, 
                             sizeof(reuse));
        if(err != 0)
        {
            throw Exception(GetError());
        }

        // normal socket setup
        m_localinfo.sin_family = AF_INET;
        m_localinfo.sin_port = htons(p_port);
        m_localinfo.sin_addr.s_addr = htonl(INADDR_ANY);
        memset(&(m_localinfo.sin_zero), 0, 8);

        // bind socket
        err = bind(m_sock, reinterpret_cast<struct sockaddr*>(&m_localinfo),
                   sizeof(struct sockaddr));
        if(err == -1)
        {
            throw Exception(GetError());
        }

        // listen
        err = listen(m_sock, 16);
        if(err == -1)
        {
            throw Exception(GetError());
        }

        m_listening = true;
    }

    DataSocket ListeningSocket::Accept()
    {
        struct sockaddr_in socketaddress;

        socklen_t size = sizeof(struct sockaddr);
        sock s = accept(m_sock, reinterpret_cast<struct sockaddr*>(&socketaddress), &size);
        if(s == -1)
        {
            throw Exception(GetError());
        }

        return DataSocket(s);
    }

    void ListeningSocket::Close()
    {
        Socket::Close();
        m_listening = false;
    }


}