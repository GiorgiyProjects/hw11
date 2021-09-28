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
    //vector<thread> threadPool;

    // first we split inputs
    vector<pair<int, int>> indices = I.GetFilePieces(file_name, NumMap);

    //first we map
    vector<map<string,int>> map_results;
    vector<map<string,int>> map_results_full_words;

    for (int i = 0; i < NumMap; ++i) {
        map_results.push_back(M.InterpretWordParts(file_name,indices[i].first, indices[i].second, N));
        map_results_full_words.push_back(M.InterpretFullWords(file_name,indices[i].first, indices[i].second, N));
    }

    // DEBUG!
    cout << "BEFORE SORT" << endl;
    cout << "Word parts" << endl;
    for (const auto& map:map_results)
    {
        for (const auto &keyval:map) {
            cout << "key: " << keyval.first << " val: " << keyval.second << endl;
        }
    }
    cout << "Full words" << endl;
    for (const auto& map:map_results_full_words)
    {
        for (const auto &keyval:map) {
            cout << "key: " << keyval.first << " val: " << keyval.second << endl;
        }
    }
    // END DEBUG!


    /*
    for (auto &thread : threadPool) {
        thread.join();
    }
    threadPool.clear();
    */

    //now we sort
    cout << "AFTER SORT" << endl;
    map<string,int> sorted_map_results = S.Sort(map_results, map_results_full_words);
    for (const auto &keyval:sorted_map_results) {
        cout << "key: " << keyval.first << " val: " << keyval.second << endl;
    }
    cout << "INDICE ARRAY" << endl;
    vector<pair<int,int>> indices_reduce = S.Shuffle(sorted_map_results, NumReduce);
    for (const auto &pair:indices_reduce)
        cout << "1: " << pair.first << " 2: " << pair.second << endl;


    //then reduce
    vector<bool> reduce_result;
    for (int i = 0; i < NumReduce; ++i) {
        reduce_result.push_back(R.Reduce(indices_reduce[i], sorted_map_results));
        //reduce_result.push_back(threadPool.emplace_back(&Reducer::Reduce,&R, indices_reduce[i], std::ref(sorted_map_results)));
    }
    for (const auto &elem:reduce_result)
        cout << "reducer result: " << elem << endl;

    /*
    for (auto &thread : threadPool) {
        thread.join();
    }*/

    //check result
    for (const auto& fl:reduce_result)
    {
        if (!fl) return false;
    }
    return true;
}
