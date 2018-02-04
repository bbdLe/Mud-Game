/*
 * @Author: bbdle 
 * @Date: 2018-02-04 13:44:55 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-04 14:22:57
 */

#include <iostream>
#include "ThreadLib/ThreadLib.h"

using namespace std;

void PrintThread(void *data)
{
    char c = reinterpret_cast<int>(data);

    for(int i = 0; i < 10000; ++i)
    {
        cout << c;
        cout.flush();
    }
}

int main(int argc, char** argv)
{
    ThreadLib::ThreadID a, b;

    a = ThreadLib::Create(PrintThread, (void*)'a');
    b = ThreadLib::Create(PrintThread, (void*)'b');

    ThreadLib::WaitForFinish(b);
    ThreadLib::WaitForFinish(a);

    return 0;
}