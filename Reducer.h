#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <fstream>
#include <cassert>
#include <sstream>
#include <cstring>
#include <experimental/filesystem>
using namespace std;

class Reducer
{
public:
    bool Reduce(pair<int,int> idx, map<string,int>& to_reduce)
    {
        if ((unsigned)idx.first >= to_reduce.size()) idx.first = to_reduce.size() - 1;
        if ((unsigned)idx.second >= to_reduce.size()) idx.second = to_reduce.size() - 1;
        if (to_reduce.size() == 0) return true;

        auto it_beg = to_reduce.begin();
        advance(it_beg, idx.first);
        auto it_end = to_reduce.begin();
        advance(it_end, idx.second);

        if (it_end->second > 1) return false;
        for(auto it = it_beg; it != it_end; it++)
        {
            if (it->second > 1) return false;
        }

        return true;
    }
};