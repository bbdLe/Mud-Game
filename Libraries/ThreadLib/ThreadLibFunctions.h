/*
 * @Author: bbdle 
 * @Date: 2018-02-03 23:54:19 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-04 00:05:11
 */

#ifndef __THREADLIBFUNCTIONS_H
#define __THREADLIBFUNCTIONS_H

#ifdef WIN32
    #include <windows.h>
    #include <map>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

#include "ThreadException.h"

namespace ThreadLib
{
    // ====================================================================
    // Standard thread function format
    // ====================================================================
    typedef void (*ThreadFunc)(void *);

    // ====================================================================
    // Standard ThreadID datatype
    // ====================================================================
    #ifdef WIN32
        typedef DWORD ThreadID;
        extern std::map<DWORD, HANDLE> g_handlemap;
    #else
        typedef pthread_t ThreadID;
    #endif

    // ====================================================================
    // Contains function pointer and data
    // ====================================================================
    class DummyData
    {
    public:
        ThreadFunc m_func;
        void* m_data;
    };

    // ====================================================================
    // Function interface depend on system
    // ====================================================================
    #ifdef WIN32
        DWORD WINAPI DummyRun(void* p_data);
    #else
        void* DummyRun(void* p_data);
    #endif
    
    // ====================================================================
    // Create Thread with function
    // ====================================================================
    inline ThreadID Create(ThreadFunc p_func, void* p_parm)
    {
        ThreadID t;
        DummyData* data = new DummyData;
        data->m_func = p_func;
        data->m_data = p_parm;

        #ifdef WIN32
            HANDLE h;
            h = CreateThread(NULL, 0, DummyRun, data, 0, &t);
            if(h != 0)
            {
                g_handlemap[t] = h;
            }
        #else
            pthread_create(&t, 0, DummyRun, data);
        #endif

        if(t == 0)
        {
            delete data;
            throw Exception(CreationFailure);
        }

        return t;
    }

    // ====================================================================
    // Get ThreadID by System API
    // ====================================================================
    inline ThreadID GetID()
    {
        #ifdef WIN32
            return GetCurrentThreadId();
        #else
            return pthread_self();
        #endif
    }

    // ====================================================================
    // Wait for thread Finish
    // ====================================================================
    inline void WaitForFinish(const ThreadID& p_thread)
    {
        #ifdef WIN32
            WaitForSingleObject(g_handlemap[p_thread], INFINITE);
            CloseHandle(g_handlemap[p_thread]);
            g_handlemap.erase(p_thread);
        #else
            pthread_join(p_thread, NULL);
        #endif
    }

    // ====================================================================
    // Kill Thread
    // ====================================================================
    inline void Kill(const ThreadID& p_thread)
    {
        #ifdef WIN32
            TerminateThread(g_handlemap[p_thread], 0);
            CloseHandle(g_handlemap[p_thread]);
            g_handlemap.erase(p_thread);
        #else
            pthread_cancel(p_thread);
        #endif
    }

    // ====================================================================
    // Return control to System
    // ====================================================================
    inline void YieldThread(int p_milliseconds = 1)
    {
        #ifdef WIN32
            Sleep(p_milliseconds);
        #else
            usleep(p_milliseconds * 1000);
        #endif
    }

}

#endif