/*
 * @Author: bbdle 
 * @Date: 2018-03-06 23:01:07 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-06 23:31:46
 */

#ifndef __ENEMY_H
#define __ENEMY_H

#include <cmath>
#include <list>
#include <iostream>
#include "BasicLib/BasicLib.h"
#include "DatabasePointer.h"
#include "Item.h"

namespace MUD
{
    typedef std::pair<item, int> loot; // 指针类, 概率

    // 敌人模板
    class EnemyTemplate : public Entity
    {
    public:
        EnemyTemplate();

        friend istream& operator>>(std::istream& p_stream, EnemyTemplate& t);

        int m_hitpoints;            //  生命
        int m_accurary;             //  准度
        int m_dodging;              //  躲避
        int m_strikedamage;         //  伤害
        int m_damageabsorb;         //  伤害吸收
        int m_experience;           //  经验值
        int m_weapon;               //  武器
        money m_moneymin;           //  掉落金钱最小值
        money m_moneymax;           //  掉落金钱最大值
        std::list<loot> m_loot;          //  留下的物品
    };

    // 真正的敌人
    class Enemy : public Entity
    {
    public:
        Enemy();

        int& HitPoints()                    { return m_hitpoints; }
        
        room& CurrentRoom()                 { return m_room; }
        
        BasicLib::sint64& NextAttackTime()  { return m_nextattacktime; }

        void LoadTemplate(enemytemplate p_template);

        friend ostream& operator<<(ostream& p_stream, const Enemy& t);
        friend istream& operator>>(istream& p_stream, Enemy& t);

        std::string Name()               { return m_template->Name(); }

        int Accuary()               { return m_template->m_accurary; }
        int Dodging()               { return m_template->m_dodging; }
        int StrikeDamage()          { return m_template->m_strikedamage; }
        int DamageAbsorb()          { return m_template->m_damageabsorb; }
        int Experience()            { return m_template->m_experience; }
        item Weapon()               { return m_template->m_weapon; }
        money MoneyMin()            { return m_template->m_moneymin; }
        money MoneyMax()            { return m_template->m_moneymax; }
        std::list<loot>& LootList()      { return m_template->m_loot; }

    private:
        enemytemplate m_template;           // 模板
        int m_hitpoints;                    // 生命值
        room m_room;                        // 敌人所在房间
        BasicLib::sint64 m_nextattacktime;  // 下次攻击时间
    };

    istream& operator>>(std::istream& p_stream, EnemyTemplate& t);
    ostream& operator<<(ostream& p_stream, const Enemy& t);
    istream& operator>>(istream& p_stream, Enemy& t);
}

#endif