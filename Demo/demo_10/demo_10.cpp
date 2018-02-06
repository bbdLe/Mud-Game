/*
 * @Author: bbdle 
 * @Date: 2018-02-06 15:21:13 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 15:34:08
 */

#include "BasicLib/BasicLib.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    BasicLib::TextLog mylogger("/home/mud_game/Demo/demo_10/test.log", "my_test_log");
    mylogger.Log("Test 1");
    mylogger.Log("Test 2");
    return 0;
}