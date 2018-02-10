#include "DatabasePointer.h"
#include "ItemDatabase.h"
#include "PlayerDatabase.h"

#define DATABASEPOINTERIMPL(pt, t, db)                  \
t& pt::operator*()                                      \
{                                                       \
    return db::get(m_id);                               \
}                                                       \
                                                        \
t* pt::operator->()                                     \
{                                                       \
    return &(db::get(m_id));                            \
}                                                       \
                                                        \
pt::operator t*()                                       \
{                                                       \
    if(m_id == 0)                                       \
        return 0;                                       \
    return &(db::get(m_id));                            \
}

namespace MUD
{
    DATABASEPOINTERIMPL(player, Player, PlayerDatabase)
    DATABASEPOINTERIMPL(item, Item, ItemDatabase)
}