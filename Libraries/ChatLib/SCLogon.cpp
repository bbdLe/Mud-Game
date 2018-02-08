/*
 * @Author: bbdle 
 * @Date: 2018-02-08 14:04:37 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 14:32:54
 */


#include "SCLogon.h"
#include "SCChat.h"
#include "SCUserDB.h"

using namespace SocketLib;

void SCLogon::Handle(string p_data)
{
    Connection<Telnet>* conn = m_connection;

    if(!UserDatabase::IsValidName(p_data))
    {
        conn->Protocol().SendString(*conn, red + bold + 
            "Sorry, that is an invalid username.\r\n" + 
            "Please enter another username:" + reset + bold);

        return;
    }

    if(UserDatabase::HasUser(p_data))
    {
        conn->Protocol().SendString(*conn, red + bold + 
            "Sorry, the name is already in use.\r\n" + 
            "Please enter another username: " + reset + bold);
        return;
    }

    UserDatabase::AddUser(conn, p_data);
    conn->Protocol().SendString(*conn, "Thank you for joining us, " + p_data + newline);
    conn->RemoveHandler();
    conn->AddHandler(new SCChat(*conn));
}

void SCLogon::Enter()
{
    m_connection->Protocol().SendString(*m_connection,
        green + bold + "Welcome To SimpleChat!\r\n" + 
        "Please enter your username: " + reset + bold);
}