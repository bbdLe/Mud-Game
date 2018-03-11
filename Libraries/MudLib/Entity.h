#ifndef __ENTITY_H
#define __ENTITY_H

#include <string>
#include "BasicLib/BasicLib.h"
#include "MudLib/MUDLogs.h"

namespace MUD
{
    typedef unsigned int entityid;

    class Entity
    {
    public:
        Entity() : m_name("UNDEFINED"), m_id(0) {}

        std::string& Name() { return m_name; }

        std::string CompName() const { return BasicLib::LowerCase(m_name); }

        bool MatchFull(const std::string& p_str) const { return CompName() == BasicLib::LowerCase(p_str); }
    
        bool Match(const std::string& p_str) const
        {
            if(p_str.empty())
            {
                return true;
            }

            std::string name = CompName();
            std::string search = BasicLib::LowerCase(p_str);

            auto pos = name.find(search);

            while(pos != std::string::npos)
            {
                if(pos == 0 || m_name[pos - 1] == ' ')
                {
                    return true;
                }

                pos = name.find(search, pos + 1);
            }

            return false;
        }

        entityid& ID() { return m_id; }

    protected:
        std::string m_name;
        entityid m_id;
    };

    class matchentityfull
    {
    public:
        matchentityfull(const std::string& p_str) : m_str(p_str) {}

        bool operator()(const Entity& p_entity) { return p_entity.MatchFull(m_str); }

        bool operator()(Entity* p_entity) { return p_entity != nullptr && p_entity->MatchFull(m_str); }

    private:
        std::string m_str;
    };

    class matchentity
    {
    public:
        matchentity(const std::string& p_str) : m_str(p_str) {}

        bool operator()(const Entity& p_entity) { return p_entity.Match(m_str); }

        bool operator()(Entity* p_entity) { return p_entity != nullptr && p_entity->Match(m_str); }
    
    private:
        std::string m_str;
    };
}

#endif