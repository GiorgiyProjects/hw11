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

class Shuffler
{
public:
    // merges vector of maps into single map
    map<string,int> Sort(vector<map<string,int>>& PartWordMaps, vector<map<string,int>>& FullWordMaps)
    {
        map<string,int> result;
        for (const auto& map:FullWordMaps)
            for (const auto& elem:map)
                result[elem.first]=elem.second;

        for (const auto& map:PartWordMaps)
            for (const auto& elem:map)
                result[elem.first]+=elem.second;

        return result;
    }

    // evenly splits data between reducers, based on number of reducers/ Every pair is start and end ids for a particular reduces to process
    vector<pair<int,int>> Shuffle(map<string,int> ToShuffle, int n)
    {
        int num_keys = ToShuffle.size();
        vector<pair<int,int>> result;
        int num_keys_per_reducer = (num_keys / n) > 0 ? (num_keys / n) : num_keys;

        for (int i = 0; i < n; i++) {
            pair<int, int> p(i * num_keys_per_reducer, (i + 1) * num_keys_per_reducer - 1); //check the eqn
            if (i == n - 1) p.second = num_keys - 1; // last reducer gets all the remaining reducers
            if (p.first > p.second) p.second = p.first;
            result.push_back(p);
        }
        return result;
    }

};