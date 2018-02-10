/*
 * @Author: bbdle 
 * @Date: 2018-02-08 23:37:35 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 20:45:17
 */

#include <fstream>
#include "BasicLib/BasicLib.h"
#include "MUDLogs.h"
#include "ItemDatabase.h"

using BasicLib::LowerCase;
using BasicLib::tostring;

namespace MUD
{
    template<> std::map<entityid, Item> EntityDatabase<Item>::m_map = std::map<entityid, Item>();

    bool ItemDatabase::Load()
    {
        std::ifstream file("/home/mud_game/Libraries/Items/items.itm");
        entityid id;
        std::string temp;

        while(file.good())
        {
            file >> temp >> id;
            m_map[id].ID() = id;
            file >> m_map[id] >> std::ws;
            USERLOG.Log( "Loaded Item: " + m_map[id].Name() );
        }

        return true;
    }
}