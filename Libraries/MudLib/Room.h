/*
 * @Author: bbdle 
 * @Date: 2018-02-25 12:04:03 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-08 23:39:06
 */

#ifndef __ROOM_H
#define __ROOM_H

#include <string>
#include <iostream>
#include <list>

#include "BasicLib/BasicLib.h"
#include "Entity.h"
#include "Item.h"
#include "Enemy.h"
#include "Attributes.h"
#include "DatabasePointer.h"

using std::list;

namespace MUD
{
    //typedef entityid enemytemplate;
    //typedef entityid enemy;

    class Room : public Entity
    {
    public:
        Room();

        RoomType& Type()        { return m_type; }
        int& Data()             { return m_data; }

        string& Description()   { return m_description; }
        entityid& Adjacent(int p_dir) { return m_rooms[p_dir]; }

        enemytemplate& SpawnWhich() { return m_spawnwhich; }
        int& MaxEnemies()       { return m_maxenemies; }

        list<item>& Items()     { return m_items; }
        money& Money()          { return m_money; }

        list<enemy>& Enemies()  { return m_enemies; }
        list<player>& Players() { return m_players; }

        void AddPlayer(player p_player);
        void RemovePlayer(player p_player);

        item FindItem(const string& p_item);
        void AddItem(item p_item);
        void RemoveItem(item p_item);

        enemy FindEnemy(const string& p_enemy);
        void AddEnemy(enemy p_enemy);
        void RemoveEnemy(enemy p_enemy);

        void LoadTemplate(std::istream& p_stream);
        void LoadData(std::istream& p_stream);
        void SaveData(std::ostream& p_stream);

    private:
        // template information
        RoomType m_type;

        int m_data;

        string m_description;

        entityid m_rooms[NUMDIRECTIONS];

        enemytemplate m_spawnwhich;

        int m_maxenemies;

        // information need to save to disk
        list<item> m_items;

        money m_money;

        // volatile data
        list<player> m_players;
        list<enemy> m_enemies;
    };
}

#endif