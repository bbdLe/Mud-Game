/*
 * @Author: bbdle 
 * @Date: 2018-02-25 17:02:13 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-25 17:03:27
 */

#ifndef __STOREDATABASE_H
#define __STOREDATABASE_H

#include <string>
#include <map>
#include "EntityDatabase.h"
#include "Store.h"

namespace MUD
{
    class StoreDatabase : public EntityDatabase<Store>
    {
    public:
        static bool Load();
    };
}

#endif