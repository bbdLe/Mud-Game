/*
 * @Author: bbdle 
 * @Date: 2018-02-06 22:25:28 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 22:50:26
 */

#ifndef __SOCKETSET_H
#define __SOCKETSET_H

#include "BasicLib/BasicLib.h"
#include "SocketLibTypes.h"
#include "SocketLibSocket.h"
#include <set>

namespace SocketLib
{
    const int MAX = FD_SETSIZE;

    class SocketSet
    {
    public:
        SocketSet();

        void AddSocket(const Socket& p_sock);

        void RemoveSocket(const Socket& p_sock);

        int Poll(long p_time = 0)
        {
            struct timeval t = {0, p_time * 1000};

            m_activityset = m_set;

            #ifdef WIN32
                return select(0, &m_activityset, 0, 0, &t);
            #else
                if(m_socketdescs.size() == 0) return 0;
                return select(*(m_socketdescs.rbegin()) + 1, &m_activityset, 0, 0, &t);
            #endif
        }

        bool HasActivity(const Socket& p_sock)
        {
            return FD_ISSET(p_sock.GetSock(), &m_activityset) != 0;
        }

    private:
        fd_set m_set;

        fd_set m_activityset;

        #ifndef WIN32
            std::set<sock> m_socketdescs;
        #endif 
    };
}

#endif