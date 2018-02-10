/*
 * @Author: bbdle 
 * @Date: 2018-02-09 23:21:08 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 16:45:28
 */

#include "BasicLib/BasicLib.h"
#include "Train.h"
#include "PlayerDatabase.h"

using namespace SocketLib;

namespace MUD
{
    void Train::Handle(string p_data)
    {
        p_data = BasicLib::LowerCase(BasicLib::ParseWord(p_data, 0));

        Player& p = *m_player;

        if(p_data == "quit")
        {
            PlayerDatabase::SavePlayer(p.ID());
            p.Conn()->RemoveHandler();
            return;
        }

        char n = p_data[0];
        if(n >= '1' && n <= '3')
        {
            if(p.StatPoints() > 0)
            {
                --p.StatPoints();
                p.AddToBaseAttr(n - '1', 1);
            }
        }

        PrintStats(true);
    }

    void Train::Enter()
    {
        Player& p = *m_player;

        p.Active() = false;

        if(p.Newbie())
        {
            p.SendString(magenta + bold + 
                "Welcome to World, " + p.Name() + "!\r\n" + 
                "You must train your character with your desired stats,\r\n" + 
                "before you enter the realm.\r\n\r\n");
            p.Newbie() = false;
        }

        PrintStats(false);
    }

    void Train::PrintStats(bool p_clear)
    {
        using BasicLib::tostring;

        Player& p = *m_player;

        if(p_clear)
        {
            p.SendString(clearscreen);
        }

        p.SendString(white + bold + 
            "--------------------------------- Your Stats ----------------------------------\r\n" +
            "Player:           " + p.Name() + "\r\n" + 
            "Level:            " + tostring(p.Level()) + "\r\n" +
            "Stat Points Left: " + tostring(p.StatPoints()) + "\r\n" + 
            "1) Strength:      " + tostring(p.GetAttr(STRENGTH)) + "\r\n" +
            "2) Health:        " + tostring(p.GetAttr(HEALTH)) + "\r\n" +
            "3) Agility:       " + tostring(p.GetAttr(AGILITY)) + "\r\n" +
            bold + 
            "-------------------------------------------------------------------------------\r\n" +
            "Enter 1, 2, or 3 to add a stat point, or \"quit\" to enter the realm: ");
    }
}