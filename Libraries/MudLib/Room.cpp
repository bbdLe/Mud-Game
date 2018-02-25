#include <string>
#include <iostream>

#include "Room.h"
#include "BasicLib/BasicLib.h"

using namespace BasicLib;
using std::string;
#include <iostream>

namespace MUD
{
    Room::Room()
    {
        m_type = PLAINROOM;
        m_data = 0;

        m_description = "UNDEFINED";

        for(int d = 0; d < NUMDIRECTIONS; ++d)
        {
            m_rooms[d] = 0;
        }

        m_spawnwhich = 0;
        m_maxenemies = 0;
        m_money = 0;
    }

    void Room::AddPlayer(player p_player)
    {
        m_players.push_back(p_player);
    }

    void Room::RemovePlayer(player p_player)
    {
        m_players.erase(std::find(m_players.begin(),
                                  m_players.end(),
                                  static_cast<entityid>(p_player)));
    }

    item Room::FindItem(const string& p_item)
    {
        std::list<item>::iterator iter = double_find_if(m_items.begin(),
                                                        m_items.end(),
                                                        matchentityfull(p_item),
                                                        matchentity(p_item));

        if(iter == m_items.end())
        {
            return 0;
        }

        return *iter;
    }

    void Room::AddItem(item p_item)
    {
        if(m_items.size() >= 32)
            m_items.pop_front();

        m_items.push_back(p_item);
    }

    void Room::RemoveItem(item p_item)
    {
        m_items.erase(std::find(m_items.begin(),
                                m_items.end(),
                                static_cast<entityid>(p_item)));
    }

    void Room::LoadTemplate(istream& p_stream)
    {
        string temp;

        p_stream >> temp >> std::ws;    std::getline(p_stream, m_name);
        p_stream >> temp >> std::ws;    std::getline(p_stream, m_description);
        p_stream >> temp >> temp;       m_type = GetRoomType(temp);
        p_stream >> temp >> m_data;

        for(int d = 0; d < NUMDIRECTIONS; ++d)
        {
            p_stream >> temp >> m_rooms[d];
        }

        p_stream >> temp >> m_spawnwhich;
        p_stream >> temp >> m_maxenemies;
    }

    void Room::LoadData(istream& p_stream)
    {
        string temp;
        p_stream >> temp;

        m_items.clear();
        entityid last;
        while(extract(p_stream , last) != 0)
        {
            m_items.push_back(last);
        }

        p_stream >> temp;
        p_stream >> m_money;
    }

    void Room::SaveData(ostream& p_stream)
    {
        p_stream << "[ITEMS] ";

        for(auto iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            p_stream << *iter << " ";
        }

        p_stream << "0\n";
        p_stream << "[MONEY] " << m_money << "\n";
    }
}