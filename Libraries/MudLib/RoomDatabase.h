/*
 * @Author: bbdle 
 * @Date: 2018-02-25 17:47:42 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-25 17:51:11
 */

#ifndef __ROOMDATABASE_H
#define __ROOMDATABASE_H

#include <cmath>
#include <string>
#include <map>
#include <set>
#include <stdexcept>

#include "EntityDatabase.h"
#include "Room.h"

namespace MUD
{
    class RoomDatabase : public EntityDatabaseVector<Room>
    {
    public:
        static void LoadTemplates();

        static void LoadData();

        static void SaveData();
    };
}

#endif