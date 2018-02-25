#include <fstream>

#include "BasicLib/BasicLib.h"
#include "MUDLogs.h"
#include "StoreDatabase.h"

namespace MUD
{
    template<> std::map<entityid, Store> EntityDatabase<Store>::m_map = std::map<entityid, Store>();
    bool StoreDatabase::Load()
    {
        std::ifstream file("/home/mud_game/Libraries/Store/stores.str");
        entityid id;
        std::string temp;

        while(file.good())
        {
            file >> temp >> id;
            m_map[id].ID() = id;
            file >> m_map[id] >> std::ws;
            USERLOG.Log("Loaded Store: " + m_map[id].Name());
        }

        return true;
    }
}