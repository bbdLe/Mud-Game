/*
 * @Author: bbdle 
 * @Date: 2018-02-06 16:09:08 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 16:45:00
 */

#ifndef __BASICLIBFUNCTIONS_H
#define __BASICLIBFUNCTIONS_H

#include <algorithm>

namespace BasicLib
{
    template<typename iterator, typename firstpass, typename secondpass>
    inline iterator double_find_if(iterator begin,
                                   iterator end,
                                   firstpass one,
                                   secondpass two)
    {
        auto iter = std::find_if(begin, end, one);
        if(iter != end)
        {
            return iter;
        }

        return std::find_if(begin, end, two);
    }

    template<typename iterator, typename firstpass, typename secondpass, class qualify>
    inline iterator double_find_if(iterator begin,
                                   iterator end,
                                   firstpass one,
                                   secondpass two,
                                   qualify q)
    {
        for(auto iter = begin; iter != end; ++iter)
        {
            if(q(*iter) && (one(*iter) || two(*iter)))
            {
                return iter;
            }
        }

        return end;
    }

    template<typename iterator, typename function, typename qualify>
    inline function operate_on_if(iterator begin,
                                  iterator end,
                                  function func,
                                  qualify q)
    {
        for(; begin != end; ++begin)
        {
            if(q(*begin))
            {
                func(*begin);
            }
        }

        return func;
    }

    template<typename datatype>
    class always
    {
    public:
        bool operator()(const datatype& p) const
        {
            return true;
        }
    };

    template<typename type>
    inline int percent(const type& first, const type& second)
    {
        return static_cast<int>(100.0 * static_cast<double>(first) / static_cast<double>(second));
    }

    inline char ASCIIToHex(char c)
    {
        if(c >= '0' && c <= '9')
        {
            return c - '0';
        }
        if(c >= 'A' && c <= 'F')
        {
            return c - 'A' + 10;
        }
        if(c >= 'a' && c <= 'f')
        {
            return c - 'a' + 10;
        }
        return 0;
    }
}

#endif