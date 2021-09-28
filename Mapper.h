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
#include <thread>
#include <mutex>
using namespace std;

class Mapper
{
public:
    // reads first n symbols from the file from strat_pos to end_pos, put into map
    void Interpret(string file_name, int start_pos, int end_pos, int n, vector<map<string,int>>& part_word_maps, vector<map<string,int>>& full_word_maps, std::mutex& mut)
    {
        map<string,int> map_result_part_word,map_result_full_word;
        std::ifstream fin (file_name);
        fin.seekg(start_pos);
        string buffer;
        buffer.resize(end_pos - start_pos + 1);

        fin.read(const_cast<char*>(buffer.c_str()), end_pos - start_pos);
        buffer[end_pos - start_pos + 1] = 0;
        std::istringstream iss(buffer);

        for (std::string word; iss >> word; )
        {
            word.resize(strlen(word.c_str())); // this will remove null pointers from the read string
            if (word.length() <= (unsigned)n) {
                map_result_full_word[word] = 1;
                continue;
            }
            map_result_part_word[word.substr(0, n)]++;
        }
        // now we write the results to vector
        {
            std::lock_guard<std::mutex> guard{mut};
            part_word_maps.push_back(map_result_part_word);
            full_word_maps.push_back(map_result_full_word);
        }
        return;
    }
};