#include "Attributes.h"

namespace MUD
{
    template<typename enumeration, int size>
    enumeration StrToEnum(string p_str, const string* strs)
    {
        p_str = BasicLib::UpperCase(p_str);

        for(int i = 0; i < size; ++i)
        {
            if(p_str == strs[i])
                return static_cast<enumeration>(i);
        }

        return static_cast<enumeration>(0);
    }

    template<typename enumeration>
    string EnumToStr(enumeration p_enum, const string* strs)
    {
        return strs[p_enum];
    }

    Attribute GetAttribute(string p_str)
    {
        return StrToEnum<Attribute, NUMATTRIBUTES>(p_str, ATTRIBUTESTRINGS);
    }

    string GetAttributeString(Attribute p_enum)
    {
        return EnumToStr<Attribute>(p_enum, ATTRIBUTESTRINGS);
    }

    ostream& operator<<(ostream& p_stream, const AttributeSet& a)
    {
        for(int i = 0; i < NUMATTRIBUTES; ++i)
        {
            p_stream << "[" << GetAttributeString(static_cast<Attribute>(i)) << "] "
                     << a.m_attributes[i] << "\n";
        }

        return p_stream;
    }

    istream& operator>>(istream& p_stream, AttributeSet& a)
    {
        std::string temp;

        for(int i = 0; i < NUMATTRIBUTES; ++i)
        {
            p_stream >> temp >> a.m_attributes[i];
        }

        return p_stream;
    }

    ItemType GetItemType(string p_str)
    {
        return StrToEnum<ItemType, NUMITEMTYPES>(p_str, ITEMSTRINGS);
    }

    string GetItemTypeString(ItemType p_enum)
    {
        return EnumToStr<ItemType>(p_enum, ITEMSTRINGS);
    }


    PlayerRank GetRank(string p_str)
    {
        return StrToEnum<PlayerRank, NUMPLAYERRANKTYPES>(p_str, PLAYERRANKSTRINGS);
    }

    string GetRankString(PlayerRank p_enum)
    {
        return EnumToStr<PlayerRank>(p_enum, PLAYERRANKSTRINGS);
    }


// ====================================================

    RoomType GetRoomType(string p_str)
    {
        return StrToEnum<RoomType, NUMROOMTYPES>(p_str, ROOMTYPESSTRINGS);
    }

    string GetRoomTypeString(RoomType p_enum)
    {
        return EnumToStr<RoomType>(p_enum, ROOMTYPESSTRINGS);
    }


// ====================================================


    int OppositeDirection(int p_dir)
    {
        return (p_dir + 2) % 4;
    }

    Direction GetDirection(string p_str)
    {
        return StrToEnum<Direction, NUMDIRECTIONS>(p_str, DIRECTIONSTRINGS);
    }

    string GetDirectionString(Direction p_enum)
    {
        return EnumToStr<Direction>(p_enum, DIRECTIONSTRINGS);
    }
    
}