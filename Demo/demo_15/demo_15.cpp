/*
 * @Author: bbdle 
 * @Date: 2018-02-08 14:19:04 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-08 15:30:52
 */

#include <pthread.h>
#include <unistd.h>
#include "SocketLib/SocketLib.h"
#include "ChatLib/ChatLib.h"
#include <iostream>

int main()
{
    using SocketLib::Telnet;

    SocketLib::ListeningManager<Telnet, SCLogon> lm;
    SocketLib::ConnectionManager<Telnet, SCLogon> cm(128);

    lm.SetConnectionManager(&cm);
    lm.AddPort(5099);

    while( 1 )
    {
        lm.Listen();
        cm.Manage();
        usleep(100000);
    }
}
