/*
 * @Author: bbdle 
 * @Date: 2018-02-06 20:02:04 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 22:00:43
 */

#ifndef __SOCKETLIBSOCKET_H
#define __SOCKETLIBSOCKET_H

#include "BasicLib/BasicLib.h"
#include "SocketLibTypes.h"
#include "SocketLibErrors.h"

namespace SocketLib
{
    // ========================================================================
    // Socket Base Class, for server or client, can't use by user directly
    // ========================================================================
    class Socket
    {
    protected:
        Socket(sock p_sock = -1);

    public:
        sock GetSock() const { return m_sock; }

        port GetLocalPort() const { return ntohs(m_localinfo.sin_port); }

        ipaddress GetLocalAddress() const { return m_localinfo.sin_addr.s_addr; }

        void ResetInfo();
        
        // close socket
        void Close();

        // make socket no blockable
        void SetBlocking(bool p_blockmode);

    private:
        bool m_isblocking;
    
    protected:
        sock m_sock;
        struct sockaddr_in m_localinfo;
    };

    // ========================================================================
    // Client Socket Class
    // ========================================================================
    class DataSocket : public Socket
    {
    public:
        DataSocket(sock p_sock = -1);

        ipaddress GetRemoteAddress() const { return m_remoteinfo.sin_addr.s_addr; }
    
        port GetRemotePort() const { return ntohs(m_remoteinfo.sin_port); }

        bool IsConnected() const { return m_connected; }

        void Connect(ipaddress p_addr, port p_port);

        int Send(const char* p_buffer, int p_size);

        int Receive(char* p_buffer, int p_size);

        void Close();

    private:
        bool m_connected;

        struct sockaddr_in m_remoteinfo;
    };

    // ========================================================================
    // Server Socket Class
    // ========================================================================
    class ListeningSocket : public Socket
    {
    public:
        ListeningSocket();

        void Listen(port p_port);

        DataSocket Accept();

        bool IsListening() const{ return m_listening; }

        void Close();

    private:
        bool m_listening;
    };
    
}
#endif