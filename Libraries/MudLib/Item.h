/*
 * @Author: bbdle 
 * @Date: 2018-02-08 23:23:38 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 17:16:41
 */

#ifndef __ITEM_H
#define __ITEM_H

#include <string>
#include <iostream>
#include "BasicLib/BasicLib.h"
#include "Entity.h"
#include "Attributes.h"

using std::ostream;
using std::istream;

namespace MUD
{
    typedef unsigned long int money;

    class Item : public Entity
    {
    public:
        Item()
        {
            m_type = WEAPON;
            m_min = 0;
            m_max = 0;
            m_speed = 0;
        }

        ItemType& Type() { return m_type; }

        int& GetAttr(int p_att)
        {
            return m_attributes[p_att];
        }

        int& Min()          { return m_min; }
        int& Max()          { return m_max; }
        int& Speed()        { return m_speed; }
        money& Price()      { return m_price; }

        friend istream& operator>>(istream& p_stream, Item& i);
    
    private:
        ItemType m_type;

        int m_min;
        
        int m_max;
        
        int m_speed;

        money m_price;

        AttributeSet m_attributes;
    };

    istream& operator>>(istream& p_stream, Item& i);
}

#endif