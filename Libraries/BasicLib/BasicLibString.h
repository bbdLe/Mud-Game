/*
 * @Author: bbdle 
 * @Date: 2018-02-04 20:57:09 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-05 00:00:47
 */

#ifndef __BASICLIBSTRING_H
#define __BASICLIBSTRING_H
#include <string>
#include <sstream>
#include "BasicLibTypes.h"

namespace BasicLib
{
    std::string UpperCase(const std::string& p_string);
    std::string LowerCase(const std::string& p_string);

    std::string SearchAndReplace(const std::string& p_target,
                                 const std::string& p_search,
                                 const std::string& p_replace);

    const std::string WHITESPACE = " \t\n\r";

    std::string TrimWhitespace(const std::string& p_string);
    std::string ParseWord(const std::string& p_string, int p_index);
    std::string RemoveWord(const std::string& p_string, int p_index);

    template<typename type>
    inline void insert(std::ostream& s, const type& t)
    {
        s << t;
    }

    template<typename type>
    inline type& extract(std::istream& s, type& t)
    {
        s >> t;
        return t;
    }

#ifdef CRAPPYCOMPILER
    template<>
    inline void insert<sint64>(std::ostream& s, const sint64& n)
    {
        char digits[21] = "01234567890123456789";
        s << _i64toa(n, digits, 10);
    }

    template<>
    inline void insert<uint64>(std::ostream& s, const uint64& n)
    {
        char digits[21] = "01234567890123456789";
        s << _ui64toa(n, digits, 10);
    }

    template<>
    inline sint64& extract<sint64>(std::istream& s, sint64& n)
    {
        // skip white space
        if(s.flags() & std::ios::skipws)
        {
            s >> std::ws;
        }

        // assert it's a number
        if(s.peek() != '-' && !isdigit(s.peek()))
        {
            s.setstate(s.failbit);
            return n;
        }

        std::string str;
        char c;

        if(s.peek() == '-')
        {
            s >> c;
            str += c;
        }

        while(isdigit(s.peek()))
        {
            s >> c;
            str += c;
        }

        n = _atoi64(str.c_str());

        return n;
    }

    template<>
    inline uint64& extract<uint64>(std::istream& s, uint64& n)
    {
        if(s.flags() & std::ios::skipws)
        {
            s >> std::ws;
        }

        if(!isdigit(s.peek()))
        {
            s.setstate(s.failbit);
            return n;
        }

        char c;
        uint64 t = 0;

        while(isdigit(s.peek()))
        {
            s >> c;
            t = t * 10 + (c - '0');
        }

        n = t;

        return n;
    }
#endif

    template<typename type>
    inline std::string tostring(const type& p_type, size_t p_width = 0)
    {
        std::stringstream str;
        insert(str, p_type);

        if(str.str().size() >= p_width)
        {
            return str.str();
        }

        return str.str() + std::string(p_width - str.str().size(), ' ');
    }

    template<typename type>
    inline type totype(const std::string& p_string)
    {
        std::stringstream str(p_string);
        type t;
        extract(str, t);
        return t;
    }
}

#endif