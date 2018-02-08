/*
 * @Author: bbdle 
 * @Date: 2018-02-08 14:04:48 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 14:34:20
 */

#include "SCUserDB.h"
#include <cctype>

using std::string;

UserDatabase::users UserDatabase::m_users;

UserDatabase::useriterator UserDatabase::find(Connection<Telnet>* p_connection)
{
    for(auto iter = m_users.begin(); iter != m_users.end(); ++iter)
    {
        if(iter->connection == p_connection)
        {
            return iter;
        }
    }

    return m_users.end();
}

bool UserDatabase::AddUser(Connection<Telnet>* p_connection, string p_name)
{
    if(!HasUser(p_name) && IsValidName(p_name))
    {
        m_users.push_back(User(p_connection, p_name));
        return true;
    }
    return false;
}

void UserDatabase::DeleteUser(Connection<Telnet>* p_connection)
{
    auto iter = find(p_connection);
    if(iter != m_users.end())
    {
        m_users.erase(iter);
    }
}

bool UserDatabase::HasUser(const string& p_name)
{
    for(auto iter = m_users.begin(); iter != m_users.end(); ++iter)
    {
        if(iter->name == p_name)
        {
            return true;
        }
    }

    return false;
}

bool UserDatabase::IsValidName(const string& p_name)
{
    const static string inv = " \"'~!@#$%^&*+/\\[]{}<>()=.,?;:";

    if(p_name.find_first_of(inv) != string::npos)
    {
        return false;
    }

    if(p_name.size() > 16 || p_name.size() < 3)
    {
        return false;
    }

    if(!isalpha(p_name[0]))
    {
        return false;
    }

    return true;
}