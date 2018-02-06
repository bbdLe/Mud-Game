/*
 * @Author: bbdle 
 * @Date: 2018-02-06 23:14:32 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 23:50:19
 */

#ifndef __SOCKETLIBSYSTEM_H
#define __SOCKETLIBSYSTEM_H

#include <string>
#include "SocketLibTypes.h"

namespace SocketLib
{
    // ========================================================================
    //  Convert string format to ipaddress(netword format)
    // ========================================================================
    ipaddress GetIPAddress(const std::string p_address);

    // ========================================================================
    //  Convert ipaddress(netword format) to string format
    // ========================================================================
    std::string GetIPString(ipaddress p_address);

    // ========================================================================
    //  Convert ipaddress to string using reverse-DNS lookup
    // ========================================================================
    std::string GetHostNameString(ipaddress p_address);

    // ========================================================================
    //  determines if a string contains a pure numerical IP address
    // ========================================================================
    bool IsIPAddress(const std::string p_address);
}

#endif