/*
 * @Author: bbdle 
 * @Date: 2018-02-25 16:15:58 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-25 23:35:31
 */

#ifndef __STORE_H
#define __STORE_H

#include <string>
#include <iostream>
#include <list>
#include <utility>

#include "BasicLib/BasicLib.h"
#include "Entity.h"
#include "Item.h"
#include "DatabasePointer.h"

using namespace BasicLib;

namespace MUD
{
    class Store : public Entity
    {
    public:
        typedef std::list<item>::iterator iterator;

        item find(const std::string& p_item)
        {
            iterator iter = BasicLib::double_find_if(begin(),
                                                     end(),
                                                     matchentityfull(p_item),
                                                     matchentity(p_item));

            if(iter != end())
                return *iter;
            
            return 0;
        }

        iterator begin()        { return m_items.begin(); }
        iterator end()          { return m_items.end(); }
        size_t size()           { return m_items.size(); }

        bool has(entityid p_item)
        {
            return std::find(begin(), end(), p_item) != end();
        }

        friend istream& operator>>(istream& p_stream, Store& s);

    private:
        std::list<item> m_items;
    };
}

#endif