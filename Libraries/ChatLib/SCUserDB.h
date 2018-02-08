/*
 * @Author: bbdle 
 * @Date: 2018-02-08 14:04:52 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 14:34:47
 */

#ifndef __SCUSERDB_H
#define __SCUSERDB_H

#include "SocketLib/SocketLib.h"
#include <string>
#include <list>

using std::string;
using SocketLib::Connection;
using SocketLib::Telnet;

class User
{
public:
    User() : connection(nullptr) {}

    User(Connection<Telnet>* c, string& n) : connection(c), name(n) {}

    string name;
    Connection<Telnet>* connection;
};

class UserDatabase
{
public:
    typedef std::list<User> users;
    typedef std::list<User>::iterator useriterator;

    static useriterator begin() { return m_users.begin(); }

    static useriterator end() { return m_users.end(); }

    static useriterator find(Connection<Telnet>* p_connection);

    static bool AddUser(Connection<Telnet>* p_connection, string name);

    static void DeleteUser(Connection<Telnet>* p_connection);

    static bool HasUser(const string& p_name);

    static bool IsValidName(const string& p_name);

private:
    static users m_users;
};

#endif