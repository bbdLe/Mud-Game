/*
 * @Author: bbdle 
 * @Date: 2018-02-09 12:19:16 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-25 23:20:05
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <cmath>
#include <string>

#include "SocketLib/SocketLib.h"
#include "BasicLib/BasicLib.h"

#include "Entity.h"
#include "Attributes.h"
#include "Item.h"
#include "DatabasePointer.h"

using SocketLib::Connection;
using SocketLib::Telnet;
using namespace BasicLib;
using namespace SocketLib;
using std::ostream;
using std::istream;
using std::string;

namespace MUD
{
    const int PLAYERITEMS = 16;

    class Player : public Entity
    {
    public:
        Player();

        static int NeedForLevel(int p_level)
        {
            return static_cast<int>(100 * (pow(1.4, p_level - 1) - 1));
        }

        int NeedForNextLevel();

        bool Train();

        int Level() { return m_level; }

        // GOD
        void BecomeStrong();

        void RecalculateStats();

        void AddHitpoints(int p_hitpoints);

        void SetHitpoints(int p_hitpoints);

        int HitPoints() const { return m_hitpoints; }

        int GetAttr(int p_attr)
        {
            int val = m_attributes[p_attr] + m_baseattributes[p_attr];

            if(p_attr == STRENGTH || p_attr == AGILITY || p_attr == HEALTH)
            {
                if(val < 1)
                    return 1;
            }

            return val;
        }

        int GetBaseAttr(int p_attr)
        {
            return m_baseattributes[p_attr];
        }

        void SetBaseAttr(int p_attr, int p_val);

        void AddToBaseAttr(int p_attr, int p_val);

        int& StatPoints() { return m_statpoints; }

        int& Experience() { return m_experience; }

        room& CurrentRoom() { return m_room; }

        money& Money() { return m_money; }

        sint64& NextAttackTime() { return m_nextattacktime; }


        
        item GetItem(int p_index) { return m_inventory[p_index]; }

        int Items() { return m_items; }

        int MaxItems() const { return PLAYERITEMS; }

        item Weapon()
        {
            if(m_weapon == -1)
            {
                return 0;
            }
            else
            {
                return m_inventory[m_weapon];
            }
        }

        item Armor()
        {
            if(m_armor == -1)
            {
                return 0;
            }
            else
            {
                return m_inventory[m_armor];
            }
        }

        void AddBonuses(item p_item);

        void AddDynamicBonuses(item p_item);

        bool PickUpItem(item p_item);

        bool DropItem(int p_index);

        void RemoveWeapon();

        void RemoveArmor();

        void UseWeapon(int p_index);

        void UseArmor(int p_index);

        int GetItemIndex(const string& p_name);

        string& Password() { return m_pass; }

        PlayerRank& Rank() { return m_rank; }

        Connection<Telnet>*& Conn() { return m_connection; }

        bool& LoggedIn() { return m_loggedin; }

        bool& Active() { return m_active; }

        bool& Newbie() { return m_newbie; }


        void SendString(const string& p_string);

        void PrintStatbar(bool p_update = false);

        friend ostream& operator<<(ostream& p_stream, const Player& p);

        friend istream& operator>>(istream& p_stream, Player& p);

    private:
        string m_pass;
        PlayerRank m_rank;

        int m_statpoints;
        int m_experience;
        int m_level;
        room m_room;
        money m_money;
        int m_hitpoints;

        AttributeSet m_baseattributes;
        AttributeSet m_attributes;

        BasicLib::sint64 m_nextattacktime;

        item m_inventory[PLAYERITEMS];
        int m_items;
        int m_weapon;
        int m_armor;

        Connection<Telnet>* m_connection;
        bool m_loggedin;
        bool m_active;
        bool m_newbie;
    };

    ostream& operator<<(ostream& p_stream, const Player& p);

    istream& operator>>(istream& p_stream, Player& p);

    class playeractive
    {
    public:
        bool operator()(Player& p_player)
        {
            return p_player.Active();
        }

        bool operator()(Player* p_player)
        {
            return p_player != nullptr && p_player->Active();
        }
    };

    class playerloggedin
    {
    public:
        bool operator()(Player& p_player)
        {
            return p_player.LoggedIn();
        }

        bool operator()(Player* p_player)
        {
            return p_player != nullptr && p_player->LoggedIn();
        }
    };

    class playersend
    {
    public:
        playersend(const string& p_msg) : m_msg(p_msg) {}

        void operator()(Player& p) { p.SendString(m_msg); }

        void operator()(Player* p)
        {
            if(p != nullptr)
            {
                p->SendString(m_msg);
            }
        }

        const string& m_msg;
    };

}



#endif