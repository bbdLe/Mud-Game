/*
 * @Author: bbdle 
 * @Date: 2018-03-06 23:40:09 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-06 23:53:50
 */

#include "Enemy.h"
#include <string>

namespace MUD
{
    EnemyTemplate::EnemyTemplate()
    {
        m_hitpoints = 0;
        m_accurary = 0;
        m_dodging = 0;
        m_strikedamage = 0;
        m_damageabsorb = 0;
        m_experience = 0;
        m_weapon = 0;
        m_moneymin = 0;
        m_moneymax = 0;
    }

    Enemy::Enemy()
    {
        m_template = 0;
        m_hitpoints = 0;
        m_room = 0;
        m_nextattacktime = 0;
    }

    void Enemy::LoadTemplate(enemytemplate p_template)
    {
        m_template = p_template;
        m_hitpoints = p_template->m_hitpoints;
        m_name = p_template->Name();
    }

    // 从配置读取敌人信息
    istream& operator>>(istream& p_stream, EnemyTemplate& t)
    {
        std::string temp;

        p_stream >> temp >> std::ws;
        std::getline(p_stream, t.m_name);

        p_stream >> temp >> t.m_hitpoints;
        p_stream >> temp >> t.m_accurary;
        p_stream >> temp >> t.m_dodging;
        p_stream >> temp >> t.m_strikedamage;
        p_stream >> temp >> t.m_damageabsorb;
        p_stream >> temp >> t.m_experience;
        p_stream >> temp >> t.m_weapon;
        p_stream >> temp >> t.m_moneymin;
        p_stream >> temp >> t.m_moneymax;

        t.m_loot.clear();
        while(BasicLib::extract(p_stream, temp) != "[ENDLOOT]")
        {
            entityid id;
            int chance;
            p_stream >> id >> chance;
            t.m_loot.push_back(loot(id, chance));
        }

        return p_stream;
    }

    ostream& operator<<(ostream& p_stream, const Enemy& t)
    {
        p_stream << "[TEMPLATEID]     " << t.m_template << "\n";
        p_stream << "[HITPOINTS]      " << t.m_hitpoints << "\n";
        p_stream << "[ROOM]           " << t.m_room << "\n";
        p_stream << "[NEXTATTACKTIME] ";
        BasicLib::insert(p_stream, t.m_nextattacktime);
        p_stream << "\n";

        return p_stream;
    }

    istream& operator>>(istream& p_stream, Enemy& t)
    {
        std::string temp;

        p_stream >> temp >> t.m_template;
        p_stream >> temp >> t.m_hitpoints;
        p_stream >> temp >> t.m_room;
        p_stream >> temp;
        BasicLib::extract(p_stream, t.m_nextattacktime);
        t.m_name = t.Name();

        return p_stream;
    }
}