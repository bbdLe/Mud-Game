/*
 * @Author: bbdle 
 * @Date: 2018-02-08 14:04:32 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 14:38:27
 */

#ifndef __SCCHAT_H
#define __SCCHAT_H

#include "SocketLib/SocketLib.h"
#include "SCUserDB.h"
#include <string>

using SocketLib::Connection;
using SocketLib::Telnet;
using std::string;

class SCChat : public Telnet::handler
{
public:
    SCChat(Connection<Telnet>& p_conn) : Telnet::handler(p_conn) {}

    void Handle(string p_data);

    void Enter();

    void Leave();

    void Hungup();

    void Flooded();

    void CloseConnection(const string& p_reason);

    void SendAll(const string& p_message);
};

#endif