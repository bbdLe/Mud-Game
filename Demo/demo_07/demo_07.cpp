/*
 * @Author: bbdle 
 * @Date: 2018-02-04 23:09:09 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-04 23:50:50
 */

#include <iostream>
#include <cassert>
#include "BasicLib/BasicLib.h"

using namespace std;

int main(int argc, char** argv)
{
    assert(BasicLib::UpperCase("Hello, World!") == "HELLO, WORLD!");
    assert(BasicLib::LowerCase("Hello, World!") == "hello, world!");
    assert(BasicLib::SearchAndReplace("Hello, bye, bye!", "bye", "World") == "Hello, World, World!");
    assert(BasicLib::TrimWhitespace(" Hello, World! ") == "Hello, World!");
    assert(BasicLib::ParseWord("a dog is a pig", 3) == "a");
    assert(BasicLib::RemoveWord("a dog is a pig", 3) == "a dog is pig");
    return 0;
}
