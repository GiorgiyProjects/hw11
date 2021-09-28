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

// based on number of input mappers will split file into parts and give offsets to mappers
class InputFairDivider
{
public:
    vector<pair<int, int>> GetFilePieces(string& file_name, int NumMap)
    {
        // first we get file size
        std::fstream fin;
        fin.open(file_name, std::ios::in|std::ios::binary);
        assert(fin.is_open());
        fin.seekg(0, std::ios::end);
        int fsize = fin.tellg();
        fin.close();
        assert(fsize > 0);
        assert(fsize > NumMap);
        int avg_block_size = fsize / NumMap;

        // now we fill out indices vector with values representing positions by which we will split into map processes
        vector<pair<int, int>> indices;
        fin.open(file_name, std::ios::in);
        // we stat with first index pair
        pair<int,int> idx(0,avg_block_size);
        fin.clear();
        fin.seekg(idx.second, ios::beg);
        // we assume that eamil size cannot be more that 30 symbols and that emails are split by newline symbol
        char buffer[30];
        fin.read(buffer, 30);
        for (const char& c:buffer)
        {
            if (c == '\n') break;
            idx.second++;
        }
        indices.push_back(idx);

        for (int i = 1; i < NumMap; i++)
        {
            pair<int,int> idx(indices[i-1].second + 1,avg_block_size * (i + 1));
            assert (idx.first < fsize);
            if (idx.second >= fsize - 1) {
                idx.second = fsize - 1;
                indices.push_back(idx);
                break;
            }
            fin.clear();
            fin.seekg(idx.second, ios::beg);
            assert(fin.tellg() != -1);

            char buffer1[30];
            fin.read(buffer1, 30);
            for (const char& c:buffer1)
            {
                if (c == '\n') break;
                idx.second++;
            }
            indices.push_back(idx);
            for (int i = 0; i < 30;i++)
            {
                buffer1[i] = '\n';
            }
        }
        return indices;
    }

    void Test(string file_name, vector<pair<int, int>> indices)
    {
        std::ifstream fin (file_name);

        for (const auto& pair:indices) {
            int start_pos = pair.first;
            int end_pos = pair.second;

            fin.seekg(start_pos);
            string buffer;
            buffer.resize(end_pos - start_pos + 1);

            fin.read(const_cast<char *>(buffer.c_str()), end_pos - start_pos);
            buffer[end_pos - start_pos + 1] = 0;
            std::istringstream iss(buffer);

            for (std::string word; iss >> word;) {
                if (word == "\n") { cout << "End of line\n"; };
                cout << word << endl;
            }
            cout << "--------" << endl;
        }
    }
};