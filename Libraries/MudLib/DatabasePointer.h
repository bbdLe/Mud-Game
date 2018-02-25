/*
 * @Author: bbdle 
 * @Date: 2018-02-09 21:01:31 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-25 20:03:41
 */

#ifndef __DATABASEPOINTER_H
#define __DATABASEPOINTER_H

#include <iostream>
#include "Entity.h"

using std::ostream;
using std::istream;

#define DATABASEPOINTER(pt, t)                          \
class t;                                                \
class pt                                                \
{                                                       \
public:                                                 \
    pt(entityid p_id = 0)                               \
        : m_id(p_id) {}                                 \
                                                        \
    pt& operator=(entityid p_id)                        \
    {                                                   \
        m_id = p_id;                                    \
        return *this;                                   \
    }                                                   \
                                                        \
    operator entityid()                                 \
    {                                                   \
        return m_id;                                    \
    }                                                   \
                                                        \
    t& operator*();                                     \
    t* operator->();                                    \
    operator t*();                                      \
                                                        \
    entityid m_id;                                      \
};                                                      \
                                                        \
inline ostream& operator<<(ostream& s, const pt& p)     \
{                                                       \
    s << p.m_id;                                        \
    return s;                                           \
}                                                       \
                                                        \
inline istream& operator>>(istream& s, pt& p)           \
{                                                       \
    s >> p.m_id;                                        \
    return s;                                           \
}

namespace MUD
{
    DATABASEPOINTER(player, Player);
    DATABASEPOINTER(item, Item);
    DATABASEPOINTER(room, Room);
}

#endif