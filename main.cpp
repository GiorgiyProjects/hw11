#include <iostream>
#include <string>
#include "MapReduce.h"

using namespace std;


int main([[maybe_unused]]int argc, char** argv) {
//

    string file = argv[1];
    int N_Map = atoi(argv[2]);
    int N_Red = atoi(argv[3]);

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