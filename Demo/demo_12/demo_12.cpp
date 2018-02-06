/*
 * @Author: bbdle 
 * @Date: 2018-02-06 16:39:01 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 22:04:50
 */

#include <iostream>
#include <cassert>
#include <vector>
#include "BasicLib/BasicLib.h"
#include "SocketLib/SocketLib.h"

using namespace std;

int main(int argc, char** argv)
{
    char buffer[] = "Hello, World\n";
    char temp[256];

    SocketLib::ListeningSocket lsock;
    lsock.Listen(9999);
    auto csock = lsock.Accept();
    csock.Send(buffer, sizeof(buffer) + 1);
    csock.Receive(temp, sizeof(temp));
    cout << temp << endl;
    lsock.Close();
    csock.Close();


    return 0;
}