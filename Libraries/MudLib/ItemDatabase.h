/*
 * @Author: bbdle 
 * @Date: 2018-02-08 23:35:04 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 23:36:27
 */

#ifndef __ITEMDATABASE_H
#define __ITEMDATABASE_H

#include <string>
#include <map>

#include "EntityDatabase.h"
#include "Item.h"

namespace MUD
{
    class ItemDatabase : public EntityDatabase<Item>
    {
    public:
        static bool Load();
    };
}

#endif