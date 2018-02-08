/*
 * @Author: bbdle 
 * @Date: 2018-02-08 14:08:17 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 14:48:23
 */

#include "SCChat.h"
#include <exception>

using namespace SocketLib;

void SCChat::Handle(string p_data)
{
    string name = UserDatabase::find(m_connection)->name;

    if(p_data[0] == '/')
    {
        string command = BasicLib::ParseWord(p_data, 0);
        string data = BasicLib::RemoveWord(p_data, 0);

        if(command == "/who")
        {
            string reply = magenta + bold + "Who is in this room: ";

            auto iter = UserDatabase::begin();
            while(iter != UserDatabase::end())
            {
                reply += iter->name;
                ++iter;
                if(iter != UserDatabase::end())
                {
                    reply += ", ";
                }
            }
            reply += newline;

            m_connection->Protocol().SendString(*m_connection, reply);
        }
        else if(command == "/quit")
        {
            CloseConnection("has quit. Message: " + data);
            m_connection->Close();
        }
    }
    else
    {
        if(BasicLib::TrimWhitespace(p_data).size() > 0)
        {
            SendAll(green + bold + "<" + name + "> : " + reset + p_data);
        }
    }
}

void SCChat::Enter()
{
    SendAll(bold + yellow + UserDatabase::find(m_connection)->name + " has entered the room.");
}

void SCChat::Leave()
{
    UserDatabase::DeleteUser(m_connection);
}

void SCChat::Hungup()
{
    CloseConnection("has hung up!");
}

void SCChat::Flooded()
{
    CloseConnection("has been kicked for flooding!");
}

void SCChat::CloseConnection(const string& p_reason)
{
    SendAll(bold + red + UserDatabase::find(m_connection)->name + " " + p_reason);
}

void SCChat::SendAll( const string& p_message )
{
    for(auto iter = UserDatabase::begin(); iter != UserDatabase::end(); ++iter)
    {
        iter->connection->Protocol().SendString(*iter->connection, p_message + newline);
    }
}
