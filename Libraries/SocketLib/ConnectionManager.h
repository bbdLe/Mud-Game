/*
 * @Author: bbdle 
 * @Date: 2018-02-07 21:56:51 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 15:24:25
 */

#ifndef __CONNECTIONMANAGER_H
#define __CONNECTIONMANAGER_H
#include <list>
#include <iostream>
#include "SocketLibTypes.h"
#include "SocketLibErrors.h"
#include "SocketSet.h"
#include "Connection.h"

namespace SocketLib
{
    template<typename protocol>
    class Connection;

    template<typename protocol, typename defaulthandler>
    class ConnectionManager
    {
    private:
        typedef std::list<Connection<protocol>> clist;
        typedef typename std::list<Connection<protocol>>::iterator clistiterator;

    public:
        ConnectionManager(int p_maxdatarate = 1024,
                          int p_sendtimeout = 60,
                          int p_maxbuffered = 8192);

        ~ConnectionManager();

        void NewConnection(DataSocket& p_sock);

        int AvailableConnections() const { return MAX - m_connections.size(); }

        int TotalConnections() const { return m_connections.size(); }

        void CloseConnections();

        void Listen();

        void Send();

        void Manage()
        {
            Listen();
            Send();
            CloseConnections();
        }

    private:
        void Close(clistiterator p_iter);

    private:
        clist m_connections;

        int m_maxdatarate;

        int m_sendtimeout;

        int m_maxbuffered;

        SocketSet m_set;
    };

    template<typename protocol, typename defaulthandler>
    ConnectionManager<protocol, defaulthandler>::
    ConnectionManager(int p_maxdatarate,
                      int p_sendtimeout,
                      int p_maxbuffered)
    {
        m_maxdatarate = p_maxdatarate;
        m_sendtimeout = p_sendtimeout;
        m_maxbuffered = p_maxbuffered;
    }

    template<typename protocol, typename defaulthandler>
    ConnectionManager<protocol, defaulthandler>::~ConnectionManager()
    {
        for(auto iter = m_connections.begin(); iter != m_connections.end(); ++iter)
        {
            iter->CloseSocket();
        }
    }

    template<typename protocol, typename defaulthandler>
    void ConnectionManager<protocol, defaulthandler>::
    NewConnection(DataSocket& p_sock)
    {
        Connection<protocol> conn(p_sock);

        if(AvailableConnections() == 0)
        {
            defaulthandler::NoRoom(conn);
            conn.CloseSocket();
        }
        else
        {
            m_connections.push_back(conn);
            Connection<protocol>& c = *m_connections.rbegin();
            c.SetBlocking(false);
            m_set.AddSocket(c);
            c.AddHandler(new defaulthandler(c));
        }
    }

    template<typename protocol, typename defaulthandler>
    void ConnectionManager<protocol, defaulthandler>::Close(clistiterator p_iter)
    {
        m_set.RemoveSocket(*p_iter);
        p_iter->CloseSocket();
        m_connections.erase(p_iter);
    }

    template<typename protocol, typename defaulthandler>
    void ConnectionManager<protocol, defaulthandler>::Listen()
    {
        if(TotalConnections() == 0)
        {
            return;
        }

        int socks = m_set.Poll();

        if(socks > 0)
        {
            for(auto iter = m_connections.begin(); iter != m_connections.end();)
            {
                auto c = iter++;

                if(m_set.HasActivity(*c))
                {
                    try
                    {
                        c->Receive();

                        if(c->GetCurrentDataRate() > m_maxdatarate)
                        {
                            c->Close();
                            c->Handler()->Flooded();
                            Close(c);
                        }


                    }
                    catch( ... )
                    {
                        c->Close();
                        c->Handler()->Hungup();
                        Close(c);
                    }
                }
            }
        }
    }

    template<typename protocol, typename defaulthandler>
    void ConnectionManager<protocol, defaulthandler>::Send()
    {
        for(auto iter = m_connections.begin(); iter != m_connections.end();)
        {
            auto c = iter++;

            try
            {
                c->SendBuffer();

                if(c->GetBufferedBytes() > m_maxbuffered || c->GetLastSendTime() > m_sendtimeout)
                {
                    c->Close();
                    c->Handler()->Hungup();
                    Close(c);
                }
            }
            catch( ... )
            {
                c->Close();
                c->Handler()->Hungup();
                Close(c);   
            }
        }
    }

    template<typename protocol, typename defaulthandler>
    void ConnectionManager<protocol, defaulthandler>::CloseConnections()
    {
        for(auto iter = m_connections.begin(); iter != m_connections.end();)
        {
            auto c = iter++;

            if(c->Closed())
            {
                Close(c);
            }
        }
    }
}

#endif