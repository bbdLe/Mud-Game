/*
 * @Author: bbdle 
 * @Date: 2018-02-08 22:03:48 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 17:15:39
 */

#ifndef __MUD_ATTRIBUTES_H
#define __MUD_ATTRIBUTES_H

#include <iostream>
#include <string>
#include <BasicLib/BasicLib.h>

using std::string;
using std::ostream;
using std::istream;

namespace MUD
{
    template<typename enumeration, int size>
    enumeration StrToEnum(string p_str, const string* strs);

    template<typename enumeration>
    string EnumToStr(enumeration p_enum, const string* strs);
    
    enum Attribute
    {
        STRENGTH        = 0,
        HEALTH          = 1,
        AGILITY         = 2,
        MAXHITPOINTS    = 3,
        ACCURACY        = 4,
        DODGING         = 5,
        STRIKEDAMAGE    = 6,
        DAMAGEABSORB    = 7,
        HPREGEN         = 8
    };

    const int NUMATTRIBUTES = 9;

    static const string ATTRIBUTESTRINGS[NUMATTRIBUTES] = 
    {
        "STRENGTH",
        "HEALTH",
        "AGILITY",
        "MAXHITPOINTS",
        "ACCURACY",
        "DODGING",
        "STRIKEDAMAGE",
        "DAMAGEABSORB",
        "HPREGEN"
    };

    Attribute GetAttribute(string p_str);

    string GetAttributeString(Attribute p_enum);

    class AttributeSet
    {
    public:
        AttributeSet()
        {
            for(int i = 0; i < NUMATTRIBUTES; ++i)
            {
                m_attributes[i] = 0;
            }
        }

        int& operator[](int p_attr)
        {
            return m_attributes[p_attr];
        }

        friend ostream& operator<<(ostream& p_stream, const AttributeSet& a);

        friend istream& operator>>(istream& p_stream, AttributeSet& a);

    private:
        int m_attributes[NUMATTRIBUTES];
    };

    ostream& operator<<(ostream& p_stream, const AttributeSet& a);

    istream& operator>>(istream& p_stream, AttributeSet& a);

    enum ItemType
    {
        WEAPON,
        ARMOR,
        HEALING
    };

    const int NUMITEMTYPES = 3;

    const string ITEMSTRINGS[NUMITEMTYPES] = 
    {
        "WEAPON",
        "ARMOR",
        "HEALING"
    };

    ItemType GetItemType(string p_str);

    string GetItemTypeString(ItemType p_enum);

    enum PlayerRank
    {
        REGULAR,
        GOD,
        ADMIN
    };

    const int NUMPLAYERRANKTYPES = 3;

    const string PLAYERRANKSTRINGS[NUMPLAYERRANKTYPES] = 
    {
        "REGULAR",
        "GOD",
        "ADMIN"
    };

    PlayerRank GetRank(string p_str);

    string GetRankString(PlayerRank p_enum);


// ====================================================

    enum RoomType
    {
        PLAINROOM,
        TRAININGROOM,
        STORE
    };

    const int NUMROOMTYPES = 3;

    const string ROOMTYPESSTRINGS[NUMROOMTYPES] = 
    {
        "PLAINROOM",
        "TRAININGROOM",
        "STORE"
    };

    RoomType GetRoomType(string p_str);

    string GetRoomTypeString(RoomType p_enum);

// ====================================================
    enum Direction
    {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    const int NUMDIRECTIONS = 4;

    int OppositeDirection(int p_dir);

    const string DIRECTIONSTRINGS[NUMDIRECTIONS] = 
    {
        "NORTH",
        "EAST",
        "SOUTH",
        "WEST"
    };

    Direction GetDirection(string p_str);

    string GetDirectionString(Direction p_enum);
}


#endif