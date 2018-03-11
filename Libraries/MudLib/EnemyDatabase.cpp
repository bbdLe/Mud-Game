/*
 * @Author: bbdle 
 * @Date: 2018-03-08 23:00:54 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-10 23:50:30
 */

#include <fstream>
#include <vector>
#include <string>
#include "BasicLib/BasicLib.h"
#include "MUDLogs.h"
#include "EnemyDatabase.h"
#include "Room.h"

namespace MUD
{
    template<> std::vector<EnemyTemplate> EntityDatabaseVector<EnemyTemplate>::m_vector = std::vector<EnemyTemplate>();
    template<> std::map<entityid, Enemy> EntityDatabase<Enemy>::m_map = std::map<entityid, Enemy>();

    void EnemyTemplateDatabase::Load()
    {
        std::ifstream file("/home/mud_game/Libraries/EnemyTemplate/enemies.templates");
        entityid id;
        std::string temp;

        while(file.good())
        {
            file >> temp >> id;

            if(m_vector.size() <= id)
            {
                m_vector.resize(id + 1);
            }

            m_vector[id].ID() = id;
            file >> m_vector[id] >> std::ws;
            USERLOG.Log("Loaded Enemy: " + m_vector[id].Name());
        }
    }

    void EnemyDatabase::Create(entityid p_template, room p_room)
    {
        entityid id = FindOpenID();
        Enemy& e = m_map[id];

        e.ID() = id;
        e.LoadTemplate(p_template);
        e.CurrentRoom() = p_room;

        p_room->AddEnemy(id);
    }

    void EnemyDatabase::Delete(enemy p_enemy)
    {
        p_enemy->CurrentRoom()->RemoveEnemy(p_enemy);
        m_map.erase(p_enemy);
    }

    void EnemyDatabase::Load()
    {
        std::ifstream file("/home/mud_game/Libraries/Enemy/enemies.instances");

        while(file.good())
        {
            entityid id;
            std::string temp;

            file >> temp >> id;
            m_map[id].ID() = id;
            file >> m_map[id] >> std::ws;
            m_map[id].CurrentRoom()->AddEnemy(id);
            USERLOG.Log("Loaded Enemy to room: " + m_map[id].Name());
        }
    }

    void EnemyDatabase::Save()
    {
        std::ofstream file("/home/mud_game/Libraries/Enemy/enemies.instances");

        for(auto iter = begin(); iter != end(); ++iter)
        {
            file << "[ID]             " << iter->ID() << "\n";
            file << *iter << "\n";
        }
    }
}