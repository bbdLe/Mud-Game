/*
 * @Author: bbdle 
 * @Date: 2018-02-06 23:46:01 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 23:55:24
 */

#include <iostream>
#include <cassert>
#include <vector>
#include "BasicLib/BasicLib.h"
#include "SocketLib/SocketLib.h"

using namespace std;

int main(int argc, char** argv)
{
    cout << SocketLib::GetIPAddress("192.168.5.1") << endl;
    cout << SocketLib::GetIPString(SocketLib::GetIPAddress("192.168.5.1")) << endl;
    cout << SocketLib::GetIPString(SocketLib::GetIPAddress("www.baidu.com")) << endl;
}