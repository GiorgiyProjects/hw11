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
#include "Mapper.h"
#include "Shuffler.h"
#include "Reducer.h"
#include "InputFairDivider.h"

bool MapReduce (int N, string& file_name, int NumMap, int NumReduce)
{
    Mapper M;
    Shuffler S;
    Reducer R;
    InputFairDivider I;
    vector<thread> threadPool;
    std::mutex mut;

    // first we split inputs
    vector<pair<int, int>> indices = I.GetFilePieces(file_name, NumMap);

    //first we map. we separate maps for full words of size n and part of words size n
    vector<map<string,int>> map_results;
    vector<map<string,int>> map_results_full_words;

    for (int i = 0; i < NumMap; ++i) {
        threadPool.emplace_back(&Mapper::Interpret,&M, file_name, indices[i].first, indices[i].second, N, std::ref(map_results), std::ref(map_results_full_words),std::ref(mut));
    }
    for (auto &thread : threadPool) {
        thread.join();
    }
    threadPool.clear();

    //now we sort
    map<string,int> sorted_map_results = S.Sort(map_results, map_results_full_words);
    vector<pair<int,int>> indices_reduce = S.Shuffle(sorted_map_results, NumReduce);

    //then reduce
    vector<bool> reduce_result;
    for (int i = 0; i < NumReduce; ++i) {
        threadPool.emplace_back(&Reducer::Reduce,&R, indices_reduce[i], std::ref(sorted_map_results), std::ref(reduce_result), std::ref(mut));
    }
    for (auto &thread : threadPool) {
        thread.join();
    }

    //check result
    for (const auto& fl:reduce_result)
    {
        if (!fl) return false;
    }
    return true;
}
