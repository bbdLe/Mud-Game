/*
 * @Author: bbdle 
 * @Date: 2018-02-09 23:01:33 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-09 23:41:40
 */

#ifndef __TRAIN_H
#define __TRAIN_H

#include <string>
#include "SocketLib/SocketLib.h"
#include "PlayerDatabase.h"

namespace MUD
{
    class Train : public Telnet::handler
    {
    public:
        Train(Connection<Telnet>& p_conn, player p_player) : Telnet::handler(p_conn)
        {
            m_player = p_player;
        }

        void Handle(string p_data);

        void Enter();

        void Leave() {}

        void Hungup() { PlayerDatabase::Logout(m_player); }

        void Flooded() { PlayerDatabase::Logout(m_player); }

        void PrintStats(bool p_clear = true);

    private:
        player m_player;
    };
}
#endif