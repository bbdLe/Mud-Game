/*
 * @Author: bbdle 
 * @Date: 2018-02-07 15:00:12 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 15:24:52
 */

#ifndef __CONNECTION_H
#define __CONNECTION_H

#include <stack>
#include "BasicLib/BasicLib.h"
#include "SocketLibTypes.h"
#include "ConnectionManager.h"
#include "SocketLibSocket.h"

namespace SocketLib
{
    const static int BUFFERSIZE = 1024;
    const static int TIMECHUNK = 16;

    template<class protocol>
    class Connection : public DataSocket
    {
    public:
        Connection();

        Connection(const DataSocket& p_socket);

        BasicLib::sint64 GetLastSendTime() const;

        BasicLib::sint64 GetLastReceiveTime() const { return m_lastReceiveTime; }

        // Like Actor, make connection manager notice this
        void Close() { m_closed = true; }

        void CloseSocket()
        {
            DataSocket::Close();

            ClearHandlers();
        }

        // put data into buffer
        void BufferData(const char* p_buffer, int p_size);

        // send buffer to client
        void SendBuffer();

        void Receive();

        int GetDataRate() const { return m_lastdatarate; }

        int GetCurrentDataRate() const { return m_datarate / TIMECHUNK; }

        int GetBufferedBytes() const { return m_sendbuffer.size(); }

        BasicLib::sint64 GetCreationTime() const { return m_creationtime; }

        protocol& Protocol() { return m_protocol; }

        bool Closed() const { return m_closed; }

        void SwitchHandler(typename protocol::handler* p_handler)
        {
            if(Handler())
            {
                Handler()->Leave();
                delete Handler();
                m_handlerstack.pop();
            }

            m_handlerstack.push(p_handler);
            p_handler->Enter();
        }

        void AddHandler(typename protocol::handler* p_handler)
        {
            if(Handler())
            {
                Handler()->Leave();
            }
            m_handlerstack.push(p_handler);
            p_handler->Enter();
        }

        void RemoveHandler()
        {
            Handler()->Leave();
            delete Handler();
            m_handlerstack.pop();
            if(Handler())
            {
                Handler()->Enter();
            }
        }

        typename protocol::handler* Handler()
        {
            if(m_handlerstack.size() == 0)
            {
                return nullptr;
            }
            return m_handlerstack.top();
        }

        void ClearHandlers()
        {
            if(Handler())
            {
                Handler()->Leave();
            }

            while(Handler())
            {
                delete Handler();
                m_handlerstack.pop();
            }
        }

    private:
        void Initialize();

    private:
        protocol m_protocol;

        std::stack<typename protocol::handler*> m_handlerstack;

        std::string m_sendbuffer;

        int m_datarate;

        int m_lastdatarate;

        BasicLib::sint64 m_lastReceiveTime;

        BasicLib::sint64 m_lastSendTime;

        BasicLib::sint64 m_creationtime;

        bool m_checksendtime;

        char m_buffer[BUFFERSIZE];

        bool m_closed;
    };

    template<class protocol>
    Connection<protocol>::Connection()
    {
        Initialize();
    }

    template<class protocol>
    Connection<protocol>::Connection(const DataSocket& p_socket) : DataSocket(p_socket)
    {
        Initialize();
    }

    template<class protocol>
    void Connection<protocol>::Initialize()
    {
        m_datarate = 0;
        m_lastdatarate = 0;
        m_lastReceiveTime = 0;
        m_lastSendTime = 0;
        m_checksendtime = false;
        m_creationtime = BasicLib::GetTimeMS();
        m_closed = false;
    }

    template<class protocol>
    BasicLib::sint64 Connection<protocol>::GetLastSendTime() const
    {
        if(m_checksendtime)
        {
            return BasicLib::GetTimeS() - m_lastSendTime;
        }
        else
        {
            return 0;
        }
    }

    template<class protocol>
    void Connection<protocol>::BufferData(const char* p_buffer, int p_size)
    {
        m_sendbuffer.append(p_buffer, p_size);
    }

    template<class protocol>
    void Connection<protocol>::SendBuffer()
    {
        if(m_sendbuffer.size() > 0)
        {
            int sent = Send(m_sendbuffer.data(), static_cast<size_t>(m_sendbuffer.size()));

            if(sent > 0)
            {
                m_sendbuffer.erase(0, sent);
                m_lastSendTime = BasicLib::GetTimeS();
                m_checksendtime = false;
            }
            else
            {
                if(!m_checksendtime)
                {
                    m_checksendtime = true;
                    m_lastSendTime = BasicLib::GetTimeS();
                }
            }
        }
    }

    template<class protocol>
    void Connection<protocol>::Receive()
    {
        int bytes = DataSocket::Receive(m_buffer, BUFFERSIZE);

        BasicLib::sint64 t = BasicLib::GetTimeS();

        if((m_lastReceiveTime / TIMECHUNK) != (t / TIMECHUNK))
        {
            m_lastdatarate = m_datarate / TIMECHUNK;
            m_datarate = 0;
            m_lastReceiveTime = t;
        }

        m_datarate += bytes;

        m_protocol.Translate(*this, m_buffer, bytes);
    }
}

#endif 