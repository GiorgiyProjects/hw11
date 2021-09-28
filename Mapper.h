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

class Mapper
{
public:
    // reads first n symbols from the file from strat_pos to end_pos, put into map
    map<string,int> InterpretWordParts(string file_name, int start_pos, int end_pos, int n)
    {
        map<string,int> map_result;
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
                continue; // skip the words of length less or equal then n
            }
            map_result[word.substr(0, n)]++;
        }
        return map_result;
    }

    map<string,int> InterpretFullWords(string file_name, int start_pos, int end_pos, int n)
    {
        map<string,int> map_result;
        std::ifstream fin (file_name);
        fin.seekg(start_pos);
        string buffer;
        buffer.resize(end_pos - start_pos + 1);

        fin.read(const_cast<char*>(buffer.c_str()), end_pos - start_pos);
        buffer[end_pos - start_pos + 1] = 0;
        std::istringstream iss(buffer);

        for (std::string word; iss >> word; )
        {
            if (word.length() > (unsigned)n) {
                continue; // skip the words of length less or equal then n
            }
            map_result[word.substr(0, n)] = 1;
        }
        return map_result;
    }
};