/*
 * @Author: bbdle 
 * @Date: 2018-02-06 12:57:14 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 13:27:24
 */

#include <iostream>
#include <cassert>
#include "BasicLib/BasicLib.h"

using namespace std;

int main(int argc, char** argv)
{
    auto random_int32a = BasicLib::Random();
    auto random_int32b = BasicLib::Random();
    assert(random_int32a != random_int32b);
    cout << "=========================" << endl;
    cout << "random_int32a : " << random_int32a << endl;
    cout << "random_int32b : " << random_int32b << endl;
    cout << "=========================" << endl;

    auto random_percent_exclusive_testa = BasicLib::RandomPercentExclusive();
    auto random_percent_exclusive_testb = BasicLib::RandomPercentExclusive();
    assert(random_percent_exclusive_testa != random_percent_exclusive_testb);
    cout << "=========================" << endl;
    cout << "random_percent_exclusive_testa : " << random_percent_exclusive_testa << endl;
    cout << "random_percent_exclusive_testb : " << random_percent_exclusive_testb << endl;
    cout << "=========================" << endl;

    auto random_range_inclusive_testa = BasicLib::RandomRange(1, 100);
    auto random_range_inclusive_testb = BasicLib::RandomRange(1, 100);
    assert(random_range_inclusive_testa != random_range_inclusive_testb);
    assert(random_range_inclusive_testa >= 1 && random_range_inclusive_testb <= 100);
    assert(random_range_inclusive_testb >= 1 && random_range_inclusive_testb <= 100);
    cout << "=========================" << endl;
    cout << "random_range_inclusive_testa : " << random_range_inclusive_testa << endl;
    cout << "random_range_inclusive_testb : " << random_range_inclusive_testb << endl;
    cout << "=========================" << endl;

    auto random_range_exclusive_testa = BasicLib::RandomRangeExclusive(1, 100);
    auto random_range_exclusive_testb = BasicLib::RandomRangeExclusive(1, 100);
    assert(random_range_exclusive_testa != random_range_exclusive_testb);
    assert(random_range_exclusive_testa >= 1 && random_range_exclusive_testa < 100);
    assert(random_range_exclusive_testb >= 1 && random_range_exclusive_testb < 100);
    cout << "=========================" << endl;
    cout << "random_range_exclusive_testa : " << random_range_exclusive_testa << endl;
    cout << "random_range_exclusive_testb : " << random_range_exclusive_testb << endl;
    cout << "=========================" << endl;

    auto random_int_testa = BasicLib::RandomInt(1, 100);
    auto random_int_testb = BasicLib::RandomInt(1, 100);
    assert(random_int_testa != random_int_testb);
    cout << "=========================" << endl;
    cout << "random_int_testa : " << random_int_testa << endl;
    cout << "random_int_testb : " << random_int_testb << endl;
    cout << "=========================" << endl;

    auto random_normal_testa = BasicLib::RandomNormal();
    auto random_normal_testb = BasicLib::RandomNormal();
    assert(random_normal_testa != random_normal_testb);
    cout << "=========================" << endl;
    cout << "random_normal_testa : " << random_normal_testa << endl;
    cout << "random_normal_testb : " << random_normal_testb << endl;
    cout << "=========================" << endl;
}
