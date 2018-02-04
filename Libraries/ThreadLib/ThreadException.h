/*
 * @Author: bbdle 
 * @Date: 2018-02-03 23:43:51 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-03 23:51:41
 */

#ifndef __THREADEXCEPTION_H
#define __THREADEXCEPTION_H

#include <exception>

#ifdef WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

namespace ThreadLib
{
    enum Error
    {
        Unspecified,
        InitFailure,
        CreationFailure
    };

    class Exception : public std::exception
    {
    public:
        // ====================================================================
        // Details: error code of the exception, with a default of Unspecified 
        // ====================================================================
        Exception(Error p_error = Unspecified) : m_error(p_error) {}

        // ====================================================================
        // Details: return error code
        // ====================================================================
        Error GetError() const
        {
            return m_error;
        }

    private:
        Error m_error;
    };
}

#endif