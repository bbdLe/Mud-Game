/*
 * @Author: bbdle 
 * @Date: 2018-03-09 23:57:33 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-10 00:25:05
 */

#ifndef __GAMELOOP_H
#define __GAMELOOP_H

#include <string>
#include "ItemDatabase.h"
#include "PlayerDatabase.h"
#include "RoomDatabase.h"
#include "StoreDatabase.h"
#include "EnemyDatabase.h"
#include "Game.h"
#include "MUDLogs.h"

using std::string;

namespace MUD
{
    class GameLoop
    {
    public:
        GameLoop()
        {
            LoadDatabases();
        }

        ~GameLoop()
        {
            SaveDatabases();
        }

        void Load();

        void LoadDatabases();

        void SaveDatabases();

        void Loop();

        void Save();

        // 战斗循环
        void PerformRound();

        // 敌人重生
        void PerformRegen();

        // 恢复体力
        void PerformHeal();

    private:
        BasicLib::sint64 m_savedatabases;       // 下次保存数据的时间
        BasicLib::sint64 m_nextround;           // 下轮战斗时间
        BasicLib::sint64 m_nextregen;           // 下轮重生时间
        BasicLib::sint64 m_nextheal;            // 下轮恢复时间
    };
}

#endif