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
        m_accuracy = 0;
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
    }

    // 从配置读取敌人信息
    istream& operator>>(istream& p_stream, EnemyTemplate& t)
    {
        std::string temp;

        p_stream >> temp >> std::ws;
        std::getline(p_stream, t.m_name);

        pstream >> temp >> t.m_hitpoints;
        pstream >> temp >> t.m_accuracy;
        pstream >> temp >> t.m_dodging;
        pstream >> temp >> t.m_strikedamage;
        pstream >> temp >> t.m_damageabsorb;
        pstream >> temp >> t.m_experience;
        pstream >> temp >> t.m_weapon;
        pstream >> temp >> t.m_moneymin;
        pstream >> temp >> t.m_moneymax;

        t.m_loot.clear();
        while(extract(p_stream, temp) != "[ENDLOOT]")
        {
            entityid id;
            int change;
            pstream >> id >> chance;
            t.m_loot.push_back(loot(id, chance));
        }

        return p_stream;
    }

    ostream& operator<<(ostream& p_stream, const Enemy& t)
    {
        // pass
    }

    istream& operator>>(istream& p_stream, Enemy& t)
    {
        // pass
    }
}