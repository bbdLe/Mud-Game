/*
 * @Author: bbdle 
 * @Date: 2018-02-06 15:01:50 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 15:19:57
 */

#ifndef __BASICLIBLOGGER_H
#define __BASICLIBLOGGER_H

#include <string>
#include <fstream>
#include "BasicLibTime.h"

namespace BasicLib
{
    class TextDecorator
    {
    public:
        static std::string FileHeader(const std::string& p_title)
        {
            return "==============================================\n" + p_title +
                   "\n==============================================\n\n";
        }

        static std::string SessionOpen()
        {
            return "\n";
        }

        static std::string SessionClose()
        {
            return "\n";
        }

        static std::string Decorate(const std::string& p_string)
        {
            return p_string + "\n";
        }
    };

    template<typename decorate>
    class Logger
    {
    public:
        Logger(const std::string& p_filename,
               const std::string& p_logtitle,
               bool p_timestamp = false,
               bool p_datestamp = false);

        ~Logger();

        void Log(const std::string& p_entry);
    
    private:
        std::fstream m_logfile;
        bool m_timestamp;
        bool m_datestamp;
    };

    typedef Logger<TextDecorator> TextLog;

    template<typename decorate>
    Logger<decorate>::Logger(const std::string& p_filename,
                             const std::string& p_logtitle,
                             bool p_timestamp,
                             bool p_datestamp)
    {
        std::fstream filetester(p_filename.c_str(), std::ios::in);

        if(filetester.is_open())
        {
            filetester.close();

            m_logfile.open(p_filename.c_str(), std::ios::out | std::ios::app);
        }
        else
        {
            m_logfile.open(p_filename.c_str(), std::ios::out);
            m_logfile << decorate::FileHeader(p_logtitle);
        }

        m_timestamp = true;
        m_datestamp = true;
        m_logfile << decorate::SessionOpen();
        Log("Session opened.");
        m_timestamp = p_timestamp;
        m_datestamp = p_timestamp;
    }

    template<typename decorate>
    Logger<decorate>::~Logger()
    {
        m_timestamp = true;
        m_datestamp = true;
        Log("Session closed.");
        m_logfile << decorate::SessionClose();
    }

    template<class decorate>
    void Logger<decorate>::Log(const std::string& p_entry)
    {
        std::string message;

        if(m_datestamp)
        {
            message += "[" + DateStamp() + "] ";
        }
        if(m_timestamp)
        {
            message += "[" + TimeStamp() + "] ";
        }

        message += p_entry;
        m_logfile << decorate::Decorate(message);
        m_logfile.flush();
    }
}

#endif