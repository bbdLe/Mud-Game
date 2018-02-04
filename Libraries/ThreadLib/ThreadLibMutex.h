/*
 * @Author: bbdle 
 * @Date: 2018-02-04 14:30:45 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-04 14:46:01
 */

#ifndef __THREADLIBMUTEX_H
#define __THREADLIBMUTEX_H

#ifdef WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif
#include <string>

namespace ThreadLib
{
    class Mutex
    {
    public:
        Mutex()
        {
            #ifdef WIN32
                InitializeCriticalSection(&m_mutex);
            #else
                pthread_mutex_init(&m_mutex, 0);
            #endif 
        }

        ~Mutex()
        {
            #ifdef WIN32
                DeleteCriticalSection(&m_mutex);
            #else
                pthread_mutex_destroy(&m_mutex);
            #endif
        }

        void Lock()
        {
            #ifdef WIN32
                EnterCriticalSection(&m_mutex);
            #else
                pthread_mutex_lock(&m_mutex);
            #endif
        }

        void Unlock()
        {
            #ifdef WIN32
                LeaveCriticalSection(&m_mutex);
            #else
                pthread_mutex_unlock(&m_mutex);
            #endif           
        }


    // Mutex Type
    private:
        #ifdef WIN32
            CRITICAL_SECTION m_mutex;
        #else
            pthread_mutex_t m_mutex;
        #endif
    };
}

#endif