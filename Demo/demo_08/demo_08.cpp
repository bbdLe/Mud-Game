/*
 * @Author: bbdle 
 * @Date: 2018-02-05 22:13:37 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-05 22:23:39
 */

#include <iostream>
#include <cassert>
#include <sstream>
#include "BasicLib/BasicLib.h"

using namespace std;

int main(int argc, char** argv)
{
    cout << BasicLib::GetTimeMS() << endl;
    cout << BasicLib::GetTimeS() << endl;
    cout << BasicLib::GetTimeM() << endl;
    cout << BasicLib::GetTimeH() << endl;
    auto timer = BasicLib::Timer();
    timer.Reset(600000);
    cout << timer.GetString() << endl;
    cout << timer.GetMS() << endl;
    cout << timer.GetS() << endl;
    cout << timer.GetM() << endl;
    cout << timer.GetH() << endl;
    cout << timer.GetD() << endl;
    cout << timer.GetY() << endl;
}
