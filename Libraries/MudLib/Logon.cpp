/*
 * @Author: bbdle 
 * @Date: 2018-02-09 21:58:40 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 21:22:50
 */

#include "Logon.h"
#include "Game.h"
#include "PlayerDatabase.h"
#include "BasicLib/BasicLib.h"

using namespace SocketLib;

namespace MUD
{
    void Logon::Handle(string p_data)
    {
        if(m_errors == 5)
        {
            m_connection->Protocol().SendString(*m_connection, 
                red + bold + "Too many incorrect responses, closing connection..." +
                newline);
            m_connection->Close();
            return;
        }

        if(m_state == NEWCONNECTION)
        {
            if(BasicLib::LowerCase(p_data) == "new")
            {
                m_state = NEWUSER;
                m_connection->Protocol().SendString(*m_connection, yellow + 
                    "Please enter your desired name: " + reset);
            }
            else
            {
                PlayerDatabase::iterator iter = PlayerDatabase::findfull(p_data);
                if(iter == PlayerDatabase::end())
                {
                    ++m_errors;
                    m_connection->Protocol().SendString(*m_connection,
                        red + bold + "Sorry, the user \"" + white + p_data + red + 
                        "\" does not exist.\r\n" + "Please enter your name, or \"new\" if you are new: " + reset);
                }
                else
                {
                    m_state = ENTERPASS;
                    m_name = p_data;
                    m_pass = iter->Password();

                    m_connection->Protocol().SendString(*m_connection,
                        green + bold + "Welcome, " + white + p_data + red + newline +
                        green + "Please enter your password: " + reset);
                }
            }

            return;
        }

        if(m_state == NEWUSER)
        {
            if(PlayerDatabase::hasfull(p_data))
            {
                m_errors++;
                m_connection->Protocol().SendString(*m_connection,
                    red + bold + "Sorry, the name \"" + white + p_data + red + 
                    "\" has already been taken." + newline + yellow + 
                    "Please enter your desired name: " + reset);
            }
            else
            {
                if(!AcceptibleName(p_data))
                {
                    ++m_errors;
                    m_connection->Protocol().SendString(*m_connection,
                        red + bold + "Sorry, the name \"" + white + p_data + red +
                        "\" has already been taken." + newline + yellow + 
                        "Please enter your desired name: " + reset);
                }
                else
                {
                    m_state = ENTERNEWPASS;
                    m_name = p_data;
                    m_connection->Protocol().SendString(*m_connection,
                        green + "Please enter your desired password: " + reset);
                }
            }

            return;
        }

        if(m_state == ENTERNEWPASS)
        {
            if(p_data.find_first_of(BasicLib::WHITESPACE) != string::npos)
            {
                ++m_errors;
                m_connection->Protocol().SendString(*m_connection,
                    red + bold + "INVALID PASSWORD!" + green + 
                    "Please enter your desired password: " + reset);
                return;
            }

            m_connection->Protocol().SendString(*m_connection,
                green + "Thank you! You are now entering the world..." + newline);

            Player p;
            p.Name() = m_name;
            p.Password() = p_data;


            USERLOG.Log(string("Now PlayerDatabase Size is ") + tostring(PlayerDatabase::size()));
            if(PlayerDatabase::size() == 0)
            {
                p.Rank() = ADMIN;
                p.ID() = 1;
            }
            else
            {
                p.ID() = PlayerDatabase::LastID() + 1;
            }

            PlayerDatabase::AddPlayer(p);

            GotoGame(true);

            return;
        }

        if(m_state == ENTERPASS)
        {
            if(m_pass == p_data)
            {
                m_connection->Protocol().SendString(*m_connection,
                    green + "Thank you! You are now entering the world" + newline);
                
                GotoGame();
            }
            else
            {
                ++m_errors;
                m_connection->Protocol().SendString(*m_connection,
                    red + bold + "INVALID PASSWORD!" + newline + yellow
                    + "Please enter your password: " + reset);
            }

            return;
        }
    }

    void Logon::Enter()
    {
        USERLOG.Log(
            GetIPString(m_connection->GetRemoteAddress()) + " - entered login state");
        
        m_connection->Protocol().SendString(*m_connection,
            red + bold + "Welcome To World\r\n" + 
            "Please enter your name, or \"new\" if you are new: " + reset);
    }

    void Logon::GotoGame(bool p_newbie)
    {
        Player& p = *PlayerDatabase::findfull(m_name);

        if(p.LoggedIn())
        {
            p.Conn()->Close();
            p.Conn()->Handler()->Hungup();
            p.Conn()->ClearHandlers();
        }

        p.Newbie() = p_newbie;
        p.Conn() = m_connection;
        p.Conn()->RemoveHandler();
        p.Conn()->AddHandler(new Game(*p.Conn(), p.ID()));
    }

    bool Logon::AcceptibleName(const string& p_name)
    {
        static string inv = " \"'~!@#$%^&*+/\\[]{}<>()=.,?;:";

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

        if(p_name == "new")
        {
            return false;
        }

        return true;
    }
}