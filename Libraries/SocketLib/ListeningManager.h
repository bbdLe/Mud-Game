/*
 * @Author: bbdle 
 * @Date: 2018-02-07 21:26:18 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 14:31:54
 */

#ifndef __LISTENINGMANAGER_H
#define __LISTENINGMANAGER_H

#include "SocketLibTypes.h"
#include "SocketLibSocket.h"
#include "SocketSet.h"
#include "SocketLibErrors.h"
#include "ConnectionManager.h"
#include <vector>

namespace SocketLib
{
    template<typename protocol, typename defaulthandler>
    class ConnectionManager;

    template<typename protocol, typename defaulthandler>
    class ListeningManager
    {
    public:
        ListeningManager();

        ~ListeningManager();

        void AddPort(port p_port);

        void SetConnectionManager(ConnectionManager<protocol, defaulthandler>* p_manager);

        void Listen();

    private:
        std::vector<ListeningSocket> m_sockets;

        SocketSet m_set;

        ConnectionManager<protocol, defaulthandler>* m_manager;
    };

    template<typename protocol, typename defaulthandler>
    ListeningManager<protocol, defaulthandler>::ListeningManager()
    {
        m_manager = nullptr;
    }

    template<typename protocol, typename defaulthandler>
    ListeningManager<protocol, defaulthandler>::~ListeningManager()
    {
        for(auto iter = m_sockets.begin(); iter != m_sockets.end(); ++iter)
        {
            iter->Close();
        }
    }

    template<typename protocol, typename defaulthandler>
    void ListeningManager<protocol, defaulthandler>::AddPort(port p_port)
    {
        if(m_sockets.size() == MAX)
        {
            throw Exception(ESocketLimitReached);
        }

        ListeningSocket lsock;
        lsock.Listen(p_port);
        lsock.SetBlocking(false);
        m_sockets.push_back(lsock);
        m_set.AddSocket(lsock);
    }

    template<typename protocol, typename defaulthandler>
    void ListeningManager<protocol, defaulthandler>::
    SetConnectionManager(ConnectionManager<protocol, defaulthandler>* p_manager)
    {
        m_manager = p_manager;
    }

    template<typename protocol, typename defaulthandler>
    void ListeningManager<protocol, defaulthandler>::Listen()
    {
        

        if(m_set.Poll() > 0)
        {
            for(auto iter = m_sockets.begin(); iter != m_sockets.end(); ++iter)
            {
                if(m_set.HasActivity(*iter))
                {
                    try
                    {
                        DataSocket datasock = iter->Accept();

                        m_manager->NewConnection(datasock);
                    }
                    catch(Exception& e)
                    {
                        if(e.ErrorCode() != EOperationWouldBlock)
                        {
                            throw e;
                        }
                    }
                }
            }
        }
    }
}

#endif