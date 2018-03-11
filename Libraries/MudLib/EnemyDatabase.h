/*
 * @Author: bbdle 
 * @Date: 2018-03-07 22:48:10 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-07 22:55:54
 */

#ifndef __ENEMYDATABASE_H
#define __ENEMYDATABASE_H

#include "EntityDatabase.h"
#include "Enemy.h"
#include "DatabasePointer.h"

namespace MUD
{
    class EnemyTemplateDatabase : public EntityDatabaseVector<EnemyTemplate>
    {
    public:
        static void Load();
    };

    class EnemyDatabase : public EntityDatabase<Enemy>
    {
    public:
        static void Create(entityid p_template, room p_room);

        static void Delete(enemy p_enemy);

        static void Load();

        static void Save();
    };
}

#endif