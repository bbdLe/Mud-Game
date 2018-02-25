/*
 * @Author: bbdle 
 * @Date: 2018-02-09 23:38:36 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-26 00:40:31
 */

#ifndef __GAME_H
#define __GAME_H

#include "SocketLib/SocketLib.h"
#include "Player.h"
#include "PlayerDatabase.h"
#include "DatabasePointer.h"
#include "MUDLogs.h"
#include "Room.h"
#include "Store.h"
#include <string>

using SocketLib::Telnet;
using SocketLib::Connection;
using std::string;

namespace MUD
{
    class Game : public Telnet::handler
    {
    public:
        Game(Connection<Telnet>& p_conn, player p_player) : Telnet::handler(p_conn)
        {
            m_player = p_player;
        }

        void Handle(string p_data);

        void Enter();

        void Leave();

        void Hungup();

        void Flooded();

        void GoToTrain();

        static void SendGlobal(const string& p_str);

        static void SendGame(const string& p_str);

        static void LogoutMessage(const string& p_reason);

        static void Announce(const string& p_announcement);

        
        
        void Whisper(const string& p_str, const string& p_player);

        static string WhoList(const string& p_who);

        static string PrintHelp(PlayerRank p_rank = REGULAR);

        string PrintStats();

        string PrintExperience();

        string PrintInventory();

        
        
        bool UseItem(const string& p_item);

        bool RemoveItem(string p_item);


        static BasicLib::Timer& GetTimer() { return s_timer; }

        static bool& Running()             { return s_running; }

        
        static string PrintRoom(room p_room);

        static void SendRoom(string p_text, room p_room);

        void Move(int p_direction);

        void GetItem(string p_item);

        void DropItem(string p_item);

        static string StoreList(entityid p_store);

        void Buy(const string& p_item);

        void Sell(const string& p_item);

    private:
        player m_player;
        string m_lastcommand;

        static BasicLib::Timer s_timer;
        static bool s_running;
    };
}

#endif