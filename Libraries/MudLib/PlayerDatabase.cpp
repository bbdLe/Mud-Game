/*
 * @Author: bbdle 
 * @Date: 2018-02-09 20:34:06 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 21:48:41
 */

#include <fstream>
#include "BasicLib/BasicLib.h"
#include "MUDLogs.h"
#include "PlayerDatabase.h"

using BasicLib::LowerCase;
using BasicLib::tostring;
using std::string;
using std::ifstream;
using std::ofstream;

namespace MUD
{
    template<> std::map<entityid, Player> EntityDatabase<Player>::m_map = std::map<entityid, Player>();

    void PlayerDatabase::LoadPlayer(string p_name)
    {
        entityid id;
        string temp;
        p_name = PlayerFileName(p_name);
        ifstream file(p_name.c_str());
        file >> temp >> id;
        m_map[id].ID() = id;
        file >> m_map[id] >> std::ws;
        USERLOG.Log("Loaded Player: " + m_map[id].Name());
    }

    void PlayerDatabase::SavePlayer(entityid p_player)
    {
        std::map<entityid, Player>::iterator iter = m_map.find(p_player);
        if(iter == m_map.end())
        {
            return;
        }

        std::string name = PlayerFileName(iter->second.Name());

        ofstream file(name.c_str());

        file << "[ID]             " << p_player << "\n";
        file << iter->second;
    }

    bool PlayerDatabase::Load()
    {
        ifstream file("/home/mud_game/Libraries/Players/players.txt", std::ifstream::in);
        string name;

        while(file.good())
        {
            file >> name >> std::ws;
            if(name.size() == 0)
            {
                break;
            }
            LoadPlayer(name);
        }
        return true;
    }

    bool PlayerDatabase::Save()
    {
        ofstream file("/home/mud_game/Libraries/Players/players.txt");
        iterator iter = begin();

        for(auto iter = begin(); iter != end(); ++iter)
        {
            file << iter->Name() << "\n";
            SavePlayer(iter->ID());
        }

        return true;
    }

    bool PlayerDatabase::AddPlayer(Player& p_player)
    {
        if(has(p_player.ID()))
        {
            return false;
        }

        if(hasfull(p_player.Name()))
        {
            return false;
        }

        m_map[p_player.ID()] = p_player;

        std::ofstream file("/home/mud_game/Libraries/Players/players.txt", std::ios::app);
        file << p_player.Name() << "\n";
        SavePlayer(p_player.ID());

        return true;
    }

    void PlayerDatabase::Logout(entityid p_player)
    {
        Player& p = get(p_player);

        USERLOG.Log(
            SocketLib::GetIPString(p.Conn()->GetRemoteAddress()) + 
            " - User " + p.Name() + " logged off." );
        
        p.Conn() = nullptr;
        p.LoggedIn() = false;
        p.Active() = false;
        SavePlayer(p_player);
    }

}