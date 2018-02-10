/*
 * @Author: bbdle 
 * @Date: 2018-02-09 20:24:13 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 20:54:38
 */

#ifndef __PLAYERDATABASE_H
#define __PLAYERDATABASE_H

#include <cmath>
#include <string>
#include <map>
#include <set>

#include "EntityDatabase.h"
#include "Player.h"

namespace MUD
{
    class PlayerDatabase : public EntityDatabase<Player>
    {
    public:
        static bool Load();
        
        static bool Save();

        static bool AddPlayer(Player& p_player);

        static std::string PlayerFileName(const std::string& p_name)
        {
            return string("/home/mud_game/Libraries/Players/" + p_name + ".plr");
        }

        static void LoadPlayer(std::string p_name);

        static void SavePlayer(entityid p_player);

        static entityid LastID()
        {
            return m_map.rbegin()->first;
        }

        static iterator findactive(const std::string& p_name)
        {
            return BasicLib::double_find_if(begin(),
                                            end(),
                                            matchentityfull(p_name),
                                            matchentity(p_name),
                                            playeractive());
        }

        static iterator findloggedin(const std::string& p_name)
        {
            return BasicLib::double_find_if(begin(),
                                            end(),
                                            matchentityfull(p_name),
                                            matchentity(p_name),
                                            playerloggedin());
        }

        static void Logout(entityid p_player);
    };
}

#endif