/*
 * @Author: bbdle 
 * @Date: 2018-02-10 15:07:56 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-10 18:03:33
 */

#include "MudLib/Player.h"
#include "MudLib/PlayerDatabase.h"
#include "MudLib/Item.h"
#include "MudLib/ItemDatabase.h"

#include "ThreadLib/ThreadLib.h"
#include "SocketLib/SocketLib.h"
#include "MudLib/Logon.h"
#include "MudLib/Game.h"
#include "MudLib/MUDLogs.h"

using namespace SocketLib;
using namespace MUD;

int main(int argc, char** argv)
{
    try
    {
        ItemDatabase::Load();
        PlayerDatabase::Load();


        ListeningManager<Telnet, Logon> lm;
        ConnectionManager<Telnet, Logon> cm(128, 60, 65536);

        lm.SetConnectionManager(&cm);
        lm.AddPort(5100);

        Game::GetTimer().Reset();
        Game::Running() = true;

        while(Game::Running())
        {
            lm.Listen();
            cm.Manage();
            ThreadLib::YieldThread();
        }
    }
    catch(SocketLib::Exception& e)
    {
        ERRORLOG.Log( "Fatal Socket Error: " + e.PrintError() );
    }
    catch(ThreadLib::Exception&)
    {
        ERRORLOG.Log( "Fatal Thread Error" );
    }
    catch(std::exception& e)
    {
        ERRORLOG.Log( "Standard Error: " + std::string( e.what() ) );
    }
    catch( ... )
    {
        ERRORLOG.Log( "Unspecified Error" );
    }
    PlayerDatabase::Save();
}
