/*
 * @Author: bbdle 
 * @Date: 2018-02-05 21:02:38 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-05 21:59:21
 */

#ifndef __BASICLIBTIME_H
#define __BASICLIBTIME_H

#include "BasicLibTypes.h"
#include <string>

namespace BasicLib
{
    // ============================================================
    // Get Time from UTC
    // ============================================================
    sint64 GetTimeMS();
    sint64 GetTimeS();
    sint64 GetTimeM();
    sint64 GetTimeH();

    // ============================================================
    // Print Timestamp in 24 hours hh:mm:ss
    // ============================================================
    std::string TimeStamp();

    // ============================================================
    // Print Timestamp in yyyy:mm:dd
    // ============================================================
    std::string DateStamp();

    class Timer
    {
    public:
        Timer();

        void Reset(sint64 p_timepassed = 0);

        sint64 GetMS() const;
        sint64 GetS() const;
        sint64 GetM() const;
        sint64 GetH() const;
        sint64 GetD() const;
        sint64 GetY() const;

        std::string GetString() const;

    private:
        sint64 m_inittime;
        sint64 m_starttime;
    };

    inline sint64 seconds(sint64 t) { return t * 1000; }
    inline sint64 minutes(sint64 t) { return t * 60 * 1000; }
    inline sint64 hours(sint64 t) { return t * 60 * 60 * 1000; }
    inline sint64 days(sint64 t) { return t * 24 * 60 * 60 * 1000; }
    inline sint64 weeks(sint64 t) { return t * 7 * 24 * 60 * 60 * 1000; }
    inline sint64 years(sint64 t) { return t * 365 * 24 * 60 * 60 * 1000; }
}

#endif