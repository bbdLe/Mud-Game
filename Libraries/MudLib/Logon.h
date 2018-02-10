#ifndef __LOGON_H
#define __LOGON_H

#include "SocketLib/SocketLib.h"
#include "MUDLogs.h"
#include <string>

using SocketLib::Telnet;
using SocketLib::Connection;
using std::string;

namespace MUD
{
    // state machine
    enum LogonState
    {
        NEWCONNECTION,
        NEWUSER,
        ENTERNEWPASS,
        ENTERPASS
    };

    class Logon : public Telnet::handler
    {
    public:
        Logon(Connection<Telnet>& p_conn) : Telnet::handler(p_conn)
        {
            m_state = NEWCONNECTION;
            m_errors = 0;
        }

        void Handle(string p_data);

        void Enter();

        void Leave() {}

        void Hungup()
        {
            USERLOG.Log(SocketLib::GetIPString(m_connection->GetRemoteAddress())
                + " - hung up in login state.");
        }

        void Flooded()
        {
            USERLOG.Log(SocketLib::GetIPString(m_connection->GetRemoteAddress())
                + " - flooded in login state.");
        }

        void GotoGame(bool p_newbie = false);

        static void NoRoom(Connection<Telnet>& p_connection)
        {
            static string msg = "Sorry, there is no more room on this server.\r\n";
            try
            {
                p_connection.Send(msg.c_str(), msg.size());
            }
            catch(SocketLib::Exception)
            {

            }
        }

        static bool AcceptibleName(const string& p_name);

    private:
        LogonState m_state;
        int m_errors;

        string m_pass;
        string m_name;
    };
}

#endif