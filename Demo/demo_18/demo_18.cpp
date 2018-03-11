/*
 * @Author: bbdle 
 * @Date: 2018-03-10 23:27:58 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-03-10 23:38:15
 */

#include <sstream>

#include "SocketLib/SocketLib.h"

#include "MudLib/Logon.h"
#include "MudLib/Game.h"
#include "MudLib/MUDLogs.h"
#include "MudLib/GameLoop.h"

#include "ThreadLib/ThreadLib.h"

using namespace SocketLib;
using namespace MUD;

int main(int argc, char **argv)
{
    try
    {
        GameLoop gameloop;

        ListeningManager<Telnet, Logon> lm;
        ConnectionManager<Telnet, Logon> cm(128, 60, 65536);

        lm.SetConnectionManager(&cm);
        lm.AddPort(5100);

        while(Game::Running())
        {
            lm.Listen();
            cm.Manage();
            gameloop.Loop();
            ThreadLib::YieldThread();
        }
        gameloop.SaveDatabases();
    }
    catch(SocketLib::Exception& e)
    {
        ERRORLOG.Log( "Fatal Socket Error: " + e.PrintError() );
    }
    catch( ThreadLib::Exception& e)
    {
        ERRORLOG.Log( "Fatal Thread Error" );
    }
    catch( std::exception& e )
    {
        ERRORLOG.Log( "Standard Error: " + std::string( e.what() ) );
    }
    catch( ... )
    {
        ERRORLOG.Log( "Unspecified Error" );
    }

}
