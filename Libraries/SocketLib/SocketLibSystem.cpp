/*
 * @Author: bbdle 
 * @Date: 2018-02-06 23:20:34 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 23:53:01
 */

#include "SocketLibSystem.h"
#include "SocketLibErrors.h"

namespace SocketLib
{
    #ifdef WIN32
        class System
        {
        public:
            System() { WSAStartup(MAKEWORD(2, 2), &m_WSAData); }

            ~System() { WSACleanup(); }
        
        private:
            WSADATA m_WSAData;
        };

        System g_system;
    #endif

    ipaddress GetIPAddress(const std::string p_address)
    {
        if(IsIPAddress(p_address))
        {
            ipaddress addr = inet_addr(p_address.c_str());

            if(addr == INADDR_NONE)
            {
                throw Exception(EDNSNotFound);
            }

            return addr;
        }
        else
        {
            // DNS Lookup
            struct hostent* host = gethostbyname(p_address.c_str());

            if(host == nullptr)
            {
                throw Exception(GetError(false));
            }

            return *(reinterpret_cast<ipaddress*>(host->h_addr));
        }
    }

    std::string GetIPString(ipaddress p_address)
    {
        char* str = inet_ntoa(*(reinterpret_cast<in_addr*>(&p_address)));
        if(str == nullptr)
        {
            return std::string("Invalid IP Address");
        }
        else
        {
            return std::string(str);
        }
    }

    std::string GetHostNameString(ipaddress p_address)
    {
        struct hostent* host = gethostbyaddr(reinterpret_cast<char*>(&p_address), 4, AF_INET);

        if(host == nullptr)
        {
            throw Exception(GetError(false));
        }
        else
        {
            return std::string(host->h_name);
        }
    }

    bool IsIPAddress(const std::string p_address)
    {
        for(auto iter = p_address.begin(); iter != p_address.end(); ++iter)
        {
            if((*iter < '0' || *iter > '9') && *iter != '.')
                return false;
        }
        return true;
    }
}