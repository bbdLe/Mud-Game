/*
 * @Author: bbdle 
 * @Date: 2018-02-08 14:04:42 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 14:35:17
 */

#ifndef __SCLOGON_H
#define __SCLOGON_H

#include "SocketLib/SocketLib.h"
#include "SCUserDB.h"
#include <string>

using SocketLib::Connection;
using SocketLib::Telnet;
using std::string;

class SCLogon : public Telnet::handler
{
public:
    SCLogon(Connection<Telnet>& p_conn) : Telnet::handler(p_conn) {}

    void Handle(string p_data);

    static void NoRoom(Connection<Telnet>& p_connection)
    {
        const static string msg = "Sorry, there is no more room on this server.\r\n";
        try
        {
            p_connection.Send(msg.c_str(), msg.size());
        }
        catch(SocketLib::Exception)
        {
            // pass
        }
    }

    void Enter();

    void Leave() {}

    void Hungup() {}

    void Flooded() {}
};

#endif