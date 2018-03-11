/*
 * @Author: bbdle 
 * @Date: 2018-03-10 00:26:17 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-10 23:51:40
 */

#include "BasicLib/BasicLib.h"
#include "GameLoop.h"
#include <fstream>
#include <string>

using namespace BasicLib;
using namespace SocketLib;

namespace MUD
{
    sint64 DBSAVETIME = minutes(15);
    sint64 ROUNDTIME  = seconds(7);
    sint64 REGENTIME  = seconds(30);
    sint64 HEALTIME   = minutes(1);

    void GameLoop::Load()
    {
        std::ifstream file("/home/mud_game/Libraries/GameData/game.data");
        file >> std::ws;

        if(file.good())
        {
            std::string temp;
            sint64 time;
            
            file >> temp;
            extract(file, time);
            Game::GetTimer().Reset(time);

            file >> temp;
            extract(file, m_savedatabases);

            file >> temp;
            extract(file, m_nextround);

            file >> temp;
            extract(file, m_nextregen);

            file >> temp;
            extract(file, m_nextheal);
        }
        else
        {
            Game::GetTimer().Reset();
            m_savedatabases = DBSAVETIME;
            m_nextround = ROUNDTIME;
            m_nextregen = REGENTIME;
            m_nextheal = HEALTIME;
        }

        Game::Running() = true;
    }

    void GameLoop::Save()
    {
        std::ofstream file("/home/mud_game/Libraries/GameData/game.data");

        file << "[GAMETIME]      ";
        insert(file, Game::GetTimer().GetMS());
        file << "\n";

        file << "[SAVEDATABASES] ";
        insert(file, m_savedatabases);
        file << "\n";

        file << "[NEXTROUND]     ";
        insert(file, m_nextround);
        file << "\n";

        file << "[NEXTREGEN]     ";
        insert(file, m_nextregen);
        file << "\n";

        file << "[NEXTHEAL]      ";
        insert(file, m_nextheal);
        file << "\n";
    }

    void GameLoop::Loop()
    {
        if(Game::GetTimer().GetMS() >= m_nextround)
        {
            PerformRound();
            m_nextround += ROUNDTIME;
        }

        if(Game::GetTimer().GetMS() >= m_nextregen)
        {
            PerformRegen();
            m_nextregen += REGENTIME;
        }

        if(Game::GetTimer().GetMS() >= m_nextheal)
        {
            PerformHeal();
            m_nextheal += HEALTIME;
        }

        if(Game::GetTimer().GetMS() >= m_savedatabases)
        {
            SaveDatabases();
            m_savedatabases += DBSAVETIME;
        }
    }

    void GameLoop::LoadDatabases()
    {
        Load();
        ItemDatabase::Load();
        PlayerDatabase::Load();
        RoomDatabase::LoadTemplates();
        RoomDatabase::LoadData();
        StoreDatabase::Load();
        EnemyTemplateDatabase::Load();
        EnemyDatabase::Load();
    }

    void GameLoop::SaveDatabases()
    {
        Save();
        PlayerDatabase::Save();
        RoomDatabase::SaveData();
        EnemyDatabase::Save();
    }

    void GameLoop::PerformRound()
    {
        sint64 now = Game::GetTimer().GetMS();

        for(auto iter = EnemyDatabase::begin(); iter != EnemyDatabase::end(); ++iter)
        {
            if(iter->CurrentRoom()->Players().size() > 0)
                Game::EnemyAttack(iter->ID());
        }
    }

    void GameLoop::PerformRegen()
    {
        for(auto iter = RoomDatabase::begin(); iter != RoomDatabase::end(); ++iter)
        {
            if(iter->SpawnWhich() != 0 && iter->Enemies().size() < iter->MaxEnemies())
            {
                EnemyDatabase::Create(iter->SpawnWhich(), iter->ID());
                Game::SendRoom(red + bold + iter->SpawnWhich()->Name() + " enters the room!", iter->ID());
            }
        }
    }

    void GameLoop::PerformHeal()
    {
        for(auto iter = PlayerDatabase::begin(); iter != PlayerDatabase::end(); ++iter)
        {
            if(iter->Active())
            {
                iter->AddHitpoints(iter->GetAttr(HPREGEN));
                iter->PrintStatbar(true);
            }
        }
    }

}