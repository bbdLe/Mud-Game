#include "Item.h"

namespace MUD
{
    istream& operator>>(istream& p_stream, Item& i)
    {
        std::string temp;

        p_stream >> temp >> std::ws;
        std::getline(p_stream, i.m_name);

        p_stream >> temp >> temp;
        i.m_type = GetItemType(temp);

        p_stream >> temp >> i.m_min;

        p_stream >> temp >> i.m_max;

        p_stream >> temp >> i.m_speed;

        p_stream >> temp >> i.m_price;

        p_stream >> i.m_attributes;

        return p_stream;
    }

}