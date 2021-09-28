#include <iostream>
#include <string>
#include "MapReduce.h"

using namespace std;


int main(int, char *[]) {
    string file = "input/test3.txt";
/*
    InputFairDivider I;
    int nmap = 4;
    vector<pair<int, int>> indices = I.GetFilePieces(file, nmap);

    for (const auto& a:indices)
    {
        cout << a.first << " " << a.second << endl;
    }
    Mapper M;

    for (int i = 0; i < nmap; ++i) {
        M.Test(file,indices[i].first,indices[i].second,nmap);
        cout << "---------\n";
        //map_results.push_back(threadPool.emplace_back(&Mapper::Interpret, &M, file_name,  indices[i].first, indices[i].second, N));

    }
*/

    int N_Map = 4;
    int N_Red = 3;

    int i = 1;
    bool flag = false;
    while (true)
    {
        flag = false;
        cout << "MapReduce cycle start" << endl;
        flag = MapReduce(i, file, N_Map, N_Red);
        cout << "MapReduce cycle finish\n" << endl;
        if (flag == true) break;
        i++;
    }

    cout << i << " is the number" << endl;

    return 0;
}