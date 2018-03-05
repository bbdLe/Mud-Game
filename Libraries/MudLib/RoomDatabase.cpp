#include <fstream>
#include <vector>
#include "BasicLib/BasicLib.h"
#include "MUDLogs.h"
#include "RoomDatabase.h"
#include <iostream>

namespace MUD
{
    template<> std::vector<Room> EntityDatabaseVector<Room>::m_vector = std::vector<Room>();

    void RoomDatabase::LoadTemplates()
    {
        std::ifstream file("/home/mud_game/Libraries/Maps/default.map");
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
            m_vector[id].LoadTemplate(file);
            file >> std::ws;
        }
    }

    void RoomDatabase::LoadData()
    {
        std::ifstream file("/home/mud_game/Libraries/Maps/default.data");

        string temp;
        entityid roomid;

        while(file.good())
        {
            file >> temp >> roomid;

            m_vector[roomid].LoadData(file);
            file >> std::ws;
        }
    }

    void RoomDatabase::SaveData()
    {
        std::ofstream file("/home/mud_game/Libraries/Maps/default.map");

        for(auto iter = begin(); iter != end(); ++iter)
        {
            file << "[ROOMID] " << iter->ID() << "\n";
            m_vector[iter->ID()].SaveData(file);
            file << "\n";
        }
    }
}