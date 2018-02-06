/*
 * @Author: bbdle 
 * @Date: 2018-02-06 16:39:01 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 16:56:07
 */

#include <iostream>
#include <cassert>
#include <vector>
#include "BasicLib/BasicLib.h"

using namespace std;

bool first_function(int a)
{
    if(a % 2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool second_function(int a)
{
    if(a % 2 != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void print_number(int a)
{
    cout << a << endl;
}

int main(int argc, char** argv)
{
    vector<int> my_vec;
    my_vec.push_back(1);
    my_vec.push_back(2);
    my_vec.push_back(3);
    my_vec.push_back(4);
    my_vec.push_back(5);
    my_vec.push_back(6);
    my_vec.push_back(7);
    my_vec.push_back(8);
    my_vec.push_back(9);
    my_vec.push_back(10);
    auto iter1 = BasicLib::double_find_if(my_vec.begin(),
                                          my_vec.end(),
                                          first_function,
                                          second_function);
    assert(*iter1 == 2);
    
    auto iter2 = BasicLib::double_find_if(my_vec.rbegin(),
                                          my_vec.rend(),
                                          second_function,
                                          first_function);
    assert(*iter2 == 9);

    auto iter3 = BasicLib::double_find_if(my_vec.begin(),
                                          my_vec.end(),
                                          first_function,
                                          second_function,
                                          [](int a) { return a > 5; });
    assert(*iter3 == 6);
    
    BasicLib::operate_on_if(my_vec.begin(),
                            my_vec.end(),
                            print_number,
                            first_function);

    assert(BasicLib::percent(634, 1000) == 63);
    
    assert(BasicLib::ASCIIToHex('F') == 15);
    assert(BasicLib::ASCIIToHex('A') == 10);
    assert(BasicLib::ASCIIToHex('B') == 11);


    return 0;
}