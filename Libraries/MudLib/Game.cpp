/*
 * @Author: bbdle 
 * @Date: 2018-02-09 23:49:20 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 21:49:50
 */

#include "Logon.h"
#include "Game.h"
#include "Train.h"
#include "ItemDatabase.h"
#include "PlayerDatabase.h"
#include "BasicLib/BasicLib.h"

using namespace SocketLib;
using namespace BasicLib;
using std::string;

namespace MUD
{
    BasicLib::Timer Game::s_timer;
    bool Game::s_running = false;

    void Game::Handle(string p_data)
    {
        Player& p = *m_player;

        if(p_data == "/")
        {
            p_data = m_lastcommand;
        }
        else
        {
            m_lastcommand = p_data;
        }

        string firstword = BasicLib::LowerCase(ParseWord(p_data, 0));

        // chat
        if(firstword == "chat" || firstword == ":")
        {
            string text = RemoveWord(p_data, 0);
            SendGame(white + bold + p.Name() + " chats: " + text);
            return;
        }

        // experience
        if(firstword == "experience" || firstword == "exp")
        {
            p.SendString(PrintExperience());
            return;
        }

        // help
        if(firstword == "help" || firstword == "commands")
        {
            p.SendString(PrintHelp(p.Rank()));
            return;
        }

        // inventory
        if(firstword == "inventory" || firstword == "inv")
        {
            p.SendString(PrintInventory());
            return;
        }

        // quit
        if(firstword == "quit")
        {
            m_connection->Close();
            LogoutMessage(p.Name() + " has left the realm.");
            return;
        }

        // remove
        if(firstword == "remove")
        {
            RemoveItem(ParseWord(p_data, 1));
            return;
        }

        // stats
        if(firstword == "stats" || firstword == "st")
        {
            p.SendString(PrintStats());
            return;
        }

        // time
        if(firstword == "time")
        {
            p.SendString(bold + cyan + 
                         "The current system time is: " + BasicLib::TimeStamp() + 
                         " on " + BasicLib::DateStamp() + "\r\nThe system has been up for: "
                         + s_timer.GetString() + ".");
            return;
        }

        // use item
        if(firstword == "use")
        {
            UseItem(RemoveWord(p_data, 0));
            return;
        }

        // chat with someone
        if(firstword == "whisper")
        {
            string name = ParseWord(p_data, 1);
            string message = RemoveWord(RemoveWord(p_data, 0), 0);

            Whisper(message, name);
            return;
        }

        // who
        if(firstword == "who")
        {
            p.SendString(WhoList(BasicLib::LowerCase(ParseWord(p_data, 1))));
            return;
        }

        // kick
        if(firstword == "kick" && p.Rank() >= GOD)
        {
            PlayerDatabase::iterator iter = PlayerDatabase::findloggedin(ParseWord(p_data, 1));

            if(iter == PlayerDatabase::end())
            {
                p.SendString(red + bold + "Player could not be found.");
                return;
            }

            if(iter->Rank() > p.Rank())
            {
                p.SendString(red + bold + "You can't kick that player.");
                return;
            }

            iter->Conn()->Close();

            LogoutMessage(iter->Name() + " has been kicked by " + p.Name() + "!!!");

            return;
        }

        // ADMIN
        if(firstword == "announce" && p.Rank() >= ADMIN)
        {
            Announce(RemoveWord(p_data, 0));
            return;
        }

        // changerank
        if(firstword == "changerank" && p.Rank() >= ADMIN)
        {
            string name = ParseWord(p_data, 1);

            PlayerDatabase::iterator iter = PlayerDatabase::find(name);

            if(iter == PlayerDatabase::end())
            {
                p.SendString(red + bold + "Error: Could not find user " + name);
                return;
            }

            PlayerRank rank = GetRank(ParseWord(p_data, 2));
            iter->Rank() = rank;
            SendGame(green + bold + iter->Name() + 
                    "'s rank has been changed to: " + GetRankString(rank));
            return;
        }

        // reload
        if(firstword == "reload" && p.Rank() >= ADMIN)
        {
            string db = BasicLib::LowerCase(ParseWord(p_data, 1));

            if(db == "items")
            {
                ItemDatabase::Load();
                p.SendString(bold + cyan + "Item Database Reloaded!");
            }
            else
            {
                p.SendString(bold + red + "Invaild Database Name");
            }

            return;
        }

        // shutdown server
        if(firstword == "shutdown" && p.Rank() >= ADMIN)
        {
            Announce("SYSTEM IS SHUTTING DOWN");
            Game::Running() = false;
            return;
        }

        SendGame(bold + p.Name() + " chats: " + p_data);
    }

    void Game::Enter()
    {
        USERLOG.Log(GetIPString(m_connection->GetRemoteAddress()) + 
                    " - User " + m_player->Name() + 
                    " entering Game state.");
        
        m_lastcommand = "";

        Player& p = *m_player;
        p.Active() = true;
        p.LoggedIn() = true;

        SendGame(bold + green + p.Name() + " has entered the world.");

        if(p.Newbie())
        {
            GoToTrain();
        }
    }

    void Game::Leave()
    {
        m_player->Active() = false;

        if(m_connection->Closed())
        {
            PlayerDatabase::Logout(m_player);
        }
    }

    void Game::Hungup()
    {
        Player& p = *m_player;
        LogoutMessage(p.Name() + " has suddenly disappeared from the realm.");
    }

    void Game::Flooded()
    {
        Player& p = *m_player;
        LogoutMessage(p.Name() + " has been kicked for flooding!");
    }

    void Game::SendGlobal(const string& p_str)
    {
        operate_on_if(PlayerDatabase::begin(),
                      PlayerDatabase::end(),
                      playersend(p_str),
                      playerloggedin());
    }

    void Game::SendGame(const string& p_str)
    {
        operate_on_if(PlayerDatabase::begin(),
                      PlayerDatabase::end(),
                      playersend(p_str),
                      playeractive());
    }

    void Game::LogoutMessage(const string& p_reason)
    {
        SendGame(red + bold + p_reason);
    }

    void Game::Announce(const string& p_announcement)
    {
        SendGlobal(cyan + bold + "System Announcement: " + p_announcement);
    }

    void Game::Whisper(const string& p_str, const string& p_player)
    {
        PlayerDatabase::iterator iter = PlayerDatabase::findactive(p_player);

        if(iter == PlayerDatabase::end())
        {
            m_player->SendString(red + bold + "Error, cannot find user.");
        }
        else
        {
            iter->SendString(yellow + m_player->Name() + " whispers to you: "
                            + reset + p_str);
            m_player->SendString(yellow + "You whisper to " + iter->Name() + 
                            ": " + reset + p_str);
        }
    }

    class wholist
    {
    public:
        string str;

        void operator()(Player& p)
        {
            str += " " + tostring(p.Name(), 17) + "| ";
            str += tostring(p.Level(), 10) + "| ";

            if(p.Active())
                str += green + "Online " + white;
            else if(p.LoggedIn())
                str += yellow + "Inactive" + white;
            else
                str += red + "Offline " + white;

            str += " | ";
            switch(p.Rank())
            {
                case REGULAR:       str += white;   break;
                case GOD:           str += yellow;  break;
                case ADMIN:         str += green;   break;
            }

            str += GetRankString(p.Rank());
            str += white + "\r\n";
        }
    };

    string Game::WhoList(const string& p_who)
    {
        using namespace BasicLib;

        string str = white + bold + 
            "--------------------------------------------------------------------------------\r\n" + 
            " Name             | Level     | Activity | Rank\r\n" +
            "--------------------------------------------------------------------------------\r\n";

        wholist who;
        if(p_who == "all")
        {
            who = operate_on_if(PlayerDatabase::begin(),
                                PlayerDatabase::end(),
                                wholist(),
                                always<Player>());
        }
        else
        {
            who = operate_on_if(PlayerDatabase::begin(),
                                PlayerDatabase::end(),
                                wholist(),
                                playerloggedin());
        }

        str += who.str;
        str += "--------------------------------------------------------------------------------";
        return str;
    }

    string Game::PrintHelp(PlayerRank p_rank)
    {
        static string help = white + bold + 
            "--------------------------------- Command List ---------------------------------\r\n" + 
            " /                          - Repeats your last command exactly.\r\n" +
            " chat <mesg>                - Sends message to everyone in the game\r\n" +
            " experience                 - Shows your experience statistics\r\n" +
            " help                       - Shows this menu\r\n" +
            " inventory                  - Shows a list of your items\r\n" + 
            " quit                       - Allows you to leave the realm.\r\n" + 
            " remove <'weapon'/'armor'>  - removes your weapon or armor\r\n" + 
            " stats                      - Shows all of your statistics\r\n" + 
            " time                       - shows the current system time.\r\n" +
            " use <item>                 - use an item in your inventory\r\n" +
            " whisper <who> <msg>        - Sends message to one person\r\n" +
            " who                        - Shows a list of everyone online\r\n" +
            " who all                    - Shows a list of everyone\r\n" +
            " look                       - Shows you the contents of a room\r\n" +
            " north/east/south/west      - Moves in a direction\r\n" +
            " get/drop <item>            - Picks up or drops an item on the ground\r\n" +
            " train                      - Train to the next level (TR)\r\n" +
            " editstats                  - Edit your statistics (TR)\r\n" +
            " list                       - Lists items in a store (ST)\r\n" +
            " buy/sell <item>            - Buy or Sell an item in a store (ST)\r\n" +
            " attack <enemy>             - Attack an enemy\r\n";

        static string god = yellow + bold + 
            "--------------------------------- God Commands ---------------------------------\r\n" +
            " kick <who>                 - kicks a user from the realm\r\n";

        static string admin = green + bold + 
            "-------------------------------- Admin Commands --------------------------------\r\n" + 
            " announce <msg>             - Makes a global system announcement\r\n" + 
            " changerank <who> <rank>    - Changes the rank of a player\r\n" + 
            " reload <db>                - Reloads the requested database\r\n" + 
            " shutdown                   - Shuts the server down\r\n";
        
        static string end = white + bold + 
            "--------------------------------------------------------------------------------";
        
        if(p_rank == REGULAR)
            return help + end;
        else if(p_rank == GOD)
            return help + god + end;
        else if(p_rank == ADMIN)
            return help + god + admin + end;
        else
            return "ERROR";
    }

    string Game::PrintStats()
    {
        Player& p = *m_player;

        return white + bold + 
            "---------------------------------- Your Stats ----------------------------------\r\n" +
            " Name:          " + p.Name() + "\r\n" +
            " Rank:          " + GetRankString(p.Rank()) + "\r\n" + 
            " HP/Max:        " + tostring(p.HitPoints()) + "/" + tostring(p.GetAttr(MAXHITPOINTS)) +
            "  (" + tostring(static_cast<int>(100*p.HitPoints()/p.GetAttr(MAXHITPOINTS))) + "%)\r\n" +
            PrintExperience() + "\r\n" + 
            " Strength:      " + tostring(p.GetAttr(STRENGTH), 16) + 
            " Accuracy:      " + tostring(p.GetAttr(ACCURACY)) + "\r\n" +
            " Health:        " + tostring(p.GetAttr(HEALTH), 16) + 
            " Dodging:       " + tostring(p.GetAttr(DODGING)) + "\r\n" +
            " Agility:       " + tostring(p.GetAttr(AGILITY), 16) + 
            " Strike Damage: " + tostring(p.GetAttr(STRIKEDAMAGE)) + "\r\n" +
            " StatPoints:    " + tostring(p.StatPoints(), 16) + 
            " Damage Absorb: " + tostring(p.GetAttr(DAMAGEABSORB)) + "\r\n" +
            "--------------------------------------------------------------------------------";
    }

    string Game::PrintExperience()
    {
        Player& p = *m_player;

        return white + bold + 
            " Level:         " + tostring(p.Level()) + "\r\n" +
            " Experience:    " + tostring(p.Experience()) + "/" +
            tostring(p.NeedForLevel(p.Level() + 1)) + " (" +
            tostring(static_cast<int>(100 * p.Experience() / p.NeedForLevel(p.Level() + 1))) +
            "%)";
    }

    string Game::PrintInventory()
    {
        Player& p = *m_player;

        string itemlist = white + bold + 
            "-------------------------------- Your Inventory --------------------------------\r\n" +
            " Items:  ";

        for(int i = 0; i < PLAYERITEMS; ++i)
        {
            if(p.GetItem(i) != 0)
            {
                itemlist += p.GetItem(i)->Name() + ", ";
            }
        }

        itemlist.erase(itemlist.size() - 2, 2);
        itemlist += "\r\n";

        itemlist += " Weapon: ";
        if(p.Weapon() == 0)
        {
            itemlist += "NONE!";
        }
        else
        {
            itemlist += p.Weapon()->Name();
        }

        itemlist += "\r\n Armor: ";
        if(p.Armor() == 0)
        {
            itemlist += "NONE!";
        }
        else
        {
            itemlist += p.Armor()->Name();
        }

        itemlist += "\r\n Money:  $" + tostring(p.Money());

        itemlist += "\r\n--------------------------------------------------------------------------------";

        return itemlist;
    }

    bool Game::UseItem(const std::string& p_item)
    {
        Player& p = *m_player;

        int i = p.GetItemIndex(p_item);

        if(i == -1)
        {
            p.SendString(red + bold + "Could not find that item!");
            return false;
        }

        Item& item = *p.GetItem(i);

        switch(item.Type())
        {
            case WEAPON:
                p.UseWeapon(i);
                return true;
            
            case ARMOR:
                p.UseArmor(i);
                return true;
            
            case HEALING:
                p.AddBonuses(item.ID());
                p.AddHitpoints(RandomInt(item.Min(), item.Max()));
                p.DropItem(i);
                return true;
        }
        return false;
    }

    bool Game::RemoveItem(string p_item)
    {
        Player& p = *m_player;

        p_item = LowerCase(p_item);

        if(p_item == "weapon" && p.Weapon() != 0)
        {
            p.RemoveWeapon();
            return true;
        }

        if(p_item == "armor" && p.Armor() != 0)
        {
            p.RemoveArmor();
            return true;
        }

        p.SendString(red + bold + "Could not Remove item!");
        return false;
    }
    
    void Game::GoToTrain()
    {
        Player& p = *m_player;

        LogoutMessage(p.Name() + " leaves to edit stats");
        m_connection->AddHandler(new Train(*m_connection, p.ID()));
    }
}