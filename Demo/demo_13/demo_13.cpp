/*
 * @Author: bbdle 
 * @Date: 2018-02-06 22:39:26 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 23:03:31
 */

#include <iostream>
#include <cassert>
#include <vector>
#include "BasicLib/BasicLib.h"
#include "SocketLib/SocketLib.h"

using namespace std;

int main(int argc, char** argv)
{
    SocketLib::SocketSet set;
    SocketLib::ListeningSocket lsock;
    lsock.Listen(9999);
    set.AddSocket(lsock);
    vector<SocketLib::DataSocket> datasock_vec;

    while(true)
    {
        int count = set.Poll(1);
        if(count != 0)
        {
            if(set.HasActivity(lsock))
            {
                auto csock = lsock.Accept();
                set.AddSocket(csock);
                datasock_vec.push_back(csock);
            }

            for(auto iter = datasock_vec.begin(); iter != datasock_vec.end(); ++iter)
            {
                if(count == 0)
                {
                    break;
                }
                
                if(set.HasActivity(*iter))
                {
                    char temp_buff[256];
                    try
                    {
                        int len = iter->Receive(temp_buff, sizeof(temp_buff));
                        iter->Send(temp_buff, len);
                        --count;
                    }
                    catch(SocketLib::Exception e)
                    {
                        if(e.ErrorCode() == SocketLib::EConnectionClosed)
                        {
                            set.RemoveSocket(*iter);
                            datasock_vec.erase(iter--);
                        }
                        else
                        {
                            throw;
                        }
                    }
                }
            }
        }
    }

    return 0;
}