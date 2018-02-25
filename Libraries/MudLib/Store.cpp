#include "Store.h"

namespace MUD
{
    istream& operator>>(istream& p_stream, Store& s)
    {
        string temp;

        p_stream >> temp >> std::ws;
        std::getline(p_stream, s.Name());

        s.m_items.clear();

        entityid last;
        p_stream >> temp;

        while(extract(p_stream, last) != 0 )
        {
            s.m_items.push_back(last);
        }

        return p_stream;
    }

}
