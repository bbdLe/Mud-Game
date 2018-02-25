/*
 * @Author: bbdle 
 * @Date: 2018-02-04 20:52:21 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-25 23:19:22
 */

#include "BasicLibTime.h"
#include "BasicLibString.h"

#include <time.h>
#ifdef WIN32
    #include "windows.h"
#else
    #include <sys/time.h>
#endif

namespace BasicLib
{
    #ifdef WIN32
        class Win32PerformanceCounter
        {
        public:
            Win32PerformanceCounter()
            {
                QueryPerformanceFrequency(static_cast<LARGE_INTEGER*>(&m_frequency));
                m_frequency = m_frequency / 1000;
            }

        private:
            sint64 m_frequency;
        };
        Win32PerformanceCounter g_win32counter;
    #endif

    sint64 GetTimeMS()
    {
        #ifdef WIN32
            sint64 t;
            QueryPerformanceCounter(static_cast<LARGE_INTEGER*>(&t));
            return t / g_win32counter.m_frequency;
        #else
            struct timeval t;
            sint64 s;

            gettimeofday(&t, 0);

            s = t.tv_sec;
            s *= 1000;
            s += t.tv_usec / 1000;

            return s;
        #endif
    }

    sint64 GetTimeS()
    {
        return GetTimeMS() / 1000;
    }

    sint64 GetTimeM()
    {
        return GetTimeMS() / 60000;
    }

    sint64 GetTimeH()
    {
        return GetTimeMS() / 3600000;
    }

    std::string TimeStamp()
    {
        char str[9];

        time_t a = time(0);
        struct tm* b = localtime(&a);
        strftime(str, 9, "%H:%M:%S", b);

        return str;
    }

    std::string DateStamp()
    {
        char str[11];

        time_t a = time(0);
        struct tm* b = localtime(&a);
        strftime(str, 11, "%Y.%m.%d", b);

        return str;
    }

    Timer::Timer()
    {
        m_starttime = 0;
        m_inittime = 0;
    }

    void Timer::Reset(sint64 p_timepassed)
    {
        m_starttime = p_timepassed;
        m_inittime = GetTimeMS();
    }

    sint64 Timer::GetMS() const
    {
        return (GetTimeMS() - m_inittime) + m_starttime;
    }

    sint64 Timer::GetS() const
    {
        return GetMS() / 1000;
    }

    sint64 Timer::GetM() const
    {
        return GetMS() / 60000;
    }

    sint64 Timer::GetH() const
    {
        return GetMS() / 3600000;
    }

    sint64 Timer::GetD() const
    {
        return GetMS() / 86400000;
    }

    sint64 Timer::GetY() const
    {
        return GetD() / 365;
    }

    std::string Timer::GetString() const
    {
        std::string str;
        sint64 y = GetY();
        sint64 d = GetD() % 365;
        sint64 h = GetH() % 24;
        sint64 m = GetM() % 60;

        if(y > 0)
            str += BasicLib::tostring(y) + " years, ";
        if(d > 0)
            str += BasicLib::tostring(d) + " days, ";
        if(h > 0)
            str += BasicLib::tostring(h) + " hours, ";
        str += BasicLib::tostring(m) + " minutes";

        return str;
    }


}