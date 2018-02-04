/*
 * @Author: bbdle 
 * @Date: 2018-02-04 14:39:18 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-04 14:47:17
 */

#include <iostream>
#include "ThreadLib/ThreadLib.h"

using namespace std;

ThreadLib::Mutex m;

void PrintThread(void *data)
{
    char c = reinterpret_cast<int>(data);

    for(int i = 0; i < 200; ++i)
    {
        m.Lock();
        for(int j = 0; j < 50; ++j)
        {
            cout << c;
            cout.flush();
        }
        m.Unlock();
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
