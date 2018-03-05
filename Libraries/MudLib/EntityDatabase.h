/*
 * @Author: bbdle 
 * @Date: 2018-02-08 21:10:13 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 17:51:16
 */

#ifndef __ENTITYDATABASE_H
#define __ENTITYDATABASE_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iterator>
#include "Entity.h"
#include <iostream>

namespace MUD
{
    template<typename datatype>
    class EntityDatabase
    {
    public:
        typedef std::map<entityid, datatype> container;

        class iterator : public container::iterator
        {
        public:
            iterator() {}

            iterator(const typename container::iterator& p_iter)
            {
                typename container::iterator& itr = *this;
                itr = p_iter;
            }

            datatype& operator*()
            {
                typename container::iterator& iter = *this;
                return iter->second;
            }

            datatype* operator->()
            {
                typename container::iterator& iter = *this;
                return &(iter->second);
            }
        };

        static iterator begin() { return iterator(m_map.begin()); }

        static iterator end()   { return iterator(m_map.end()); }

        static iterator find(entityid p_id) { return iterator(m_map.find(p_id)); }

        static iterator findfull(const std::string& p_name) { return std::find_if(begin(), end(), matchentityfull(p_name)); }

        static iterator find(const std::string& p_name)
        {
            return BasicLib::double_find_if(begin(),
                                            end(),
                                            matchentityfull(p_name),
                                            matchentity(p_name));
        }

        static datatype& get(entityid p_id)
        {
            auto iter = find(p_id);
            if(iter == end())
            {
                throw std::exception();
            }

            return *iter;
        }

        static bool has(entityid p_id) { return find(p_id) != end(); }

        static bool hasfull(std::string p_name) { return findfull(p_name) != end(); }

        static bool has(std::string p_name) { return find(p_name) != end(); }

        static size_t size() { return m_map.size(); }

        entityid FindOpenID()
        {
            if(m_map.size() == 0)
            {
                return 1;
            }
                

            if(m_map.size() == m_map.rbegin()->first)
            {
                return m_map.size() + 1;
            }
            
            entityid openid = 0;
            entityid previous = 0;

            for(auto iter = m_map.begin(); iter != m_map.end(); ++iter)
            {
                if(openid != 0)
                {
                    break;
                }

                if(iter->first != previous + 1)
                {
                    openid = previous + 1;
                }

                previous = iter->first;
            }
        }
    
    public:
        static std::map<entityid, datatype> m_map;
    };

    template<typename datatype>
    class EntityDatabaseVector
    {
    public:
        typedef typename std::vector<datatype>::iterator iterator;

        static iterator begin() { return m_vector.begin() + 1; }

        static iterator end() { return m_vector.end(); }

        static size_t size() { return m_vector.size() - 1; }

        static datatype& get(entityid p_id)
        {
            if(p_id >= m_vector.size() || p_id == 0)
            {
                throw std::exception();
            }

            return m_vector[p_id];
        }

    public:
        static std::vector<datatype> m_vector;
    };
}

#endif