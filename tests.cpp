#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/included/execution_monitor.hpp>
#include "MapReduce.h"
using namespace std;

struct ArgsFixture {
    ArgsFixture(): argc(boost::unit_test::framework::master_test_suite().argc),
                   argv(boost::unit_test::framework::master_test_suite().argv){}
    int argc;
    char **argv;
};

BOOST_AUTO_TEST_SUITE( TestSuite )

    struct cout_redirect {
        cout_redirect( std::streambuf * new_buffer )
                : old( std::cout.rdbuf( new_buffer ) )
        { }

        ~cout_redirect( ) {
            std::cout.rdbuf( old );
        }

    private:
        std::streambuf * old;
    };

    BOOST_FIXTURE_TEST_CASE ( input_fair_divider_test1, ArgsFixture ) {
        {
            string file = "/home/bbrave/Desktop/OTUS/homeworks/hw11/input/test1.txt";
            int N_Map = 3;
            InputFairDivider I;
            vector<pair<int, int>> indices = I.GetFilePieces(file, N_Map);
            I.Test(file, indices);

            file = "/home/bbrave/Desktop/OTUS/homeworks/hw11/input/test3.txt";
            indices.clear();
            cout << "nmap=1" <<endl;
            indices = I.GetFilePieces(file, 1);
            I.Test(file, indices);
            cout << "nmap=3" <<endl;
            indices = I.GetFilePieces(file, 3);
            I.Test(file, indices);
            cout << "nmap=5" <<endl;
            indices = I.GetFilePieces(file, 5);
            I.Test(file, indices);

            BOOST_CHECK(1 == 1); // this is not really a test - just want to see everything looks okay
        }
    }

    BOOST_FIXTURE_TEST_CASE ( test_mapper_and_shuffler1, ArgsFixture ) {
        {
            string file = "/home/bbrave/Desktop/OTUS/homeworks/hw11/input/test1.txt";
            int N_Map = 3;
            InputFairDivider I;
            Shuffler S;
            Mapper M;

            vector<pair<int, int>> indices = I.GetFilePieces(file, N_Map);
            vector<map<string,int>> map_results;
            vector<map<string,int>> map_results_full_words;
            mutex mut;

            for (int i = 0; i < N_Map; ++i) {
                M.Interpret(file,indices[i].first, indices[i].second, 3, map_results, map_results_full_words, mut);
            }
            cout << "Result map" << endl;
            map<string,int> sorted_map_results = S.Sort(map_results, map_results_full_words);
            for (const auto &keyval:sorted_map_results) {
                cout << "key: " << keyval.first << " val: " << keyval.second << endl;
            }
            map<string,int> expected_result;
            expected_result["one"] = 1;

            BOOST_CHECK(expected_result == sorted_map_results);
        }
    }

    BOOST_FIXTURE_TEST_CASE ( test_mapper_and_shuffler2, ArgsFixture ) {
        {
            string file = "/home/bbrave/Desktop/OTUS/homeworks/hw11/input/test2.txt";
            int N_Map = 3;
            InputFairDivider I;
            Shuffler S;
            Mapper M;

            vector<pair<int, int>> indices = I.GetFilePieces(file, N_Map);
            vector<map<string,int>> map_results;
            vector<map<string,int>> map_results_full_words;
            mutex mut;

            for (int i = 0; i < N_Map; ++i) {
                M.Interpret(file,indices[i].first, indices[i].second, 7, map_results, map_results_full_words, mut);
            }

            cout << "Result map" << endl;
            map<string,int> sorted_map_results = S.Sort(map_results, map_results_full_words);
            for (const auto &keyval:sorted_map_results) {
                cout << "key: " << keyval.first << " val: " << keyval.second << endl;
            }
            map<string,int> expected_result;
            expected_result["one"] = 1;
            expected_result["one1"] = 1;
            expected_result["one11"] = 1;
            expected_result["one111"] = 1;
            expected_result["one1111"] = 6;

            BOOST_CHECK(expected_result == sorted_map_results);
        }
    }

    BOOST_FIXTURE_TEST_CASE ( test_reduce, ArgsFixture ) {
        {
            map<string,int> to_reduce;
            to_reduce["one"] = 1;
            to_reduce["one1"] = 1;
            to_reduce["one11"] = 1;
            to_reduce["one111"] = 1;
            to_reduce["one1111"] = 5;
            Reducer R;
            vector<bool> result;
            mutex mut;

            R.Reduce(pair<int,int>(0,3), to_reduce, result, mut);
            BOOST_CHECK(result[0] == true);
            R.Reduce(pair<int,int>(3,5),to_reduce, result, mut);
            BOOST_CHECK( result[1] == false);
            map<string,int> to_reduce2;
            R.Reduce(pair<int,int>(0,3),to_reduce2,result, mut);
            BOOST_CHECK( result[2] == true);

        }
    }

    BOOST_FIXTURE_TEST_CASE ( TestMapReduce1, ArgsFixture ) {
        {
            string file = "/home/bbrave/Desktop/OTUS/homeworks/hw11/input/test1.txt";
            int N_Map = 3;
            int N_Red = 2;
            bool flag = MapReduce(2, file, N_Map, N_Red);
            BOOST_CHECK(flag == false);
            flag = MapReduce(3, file, N_Map, N_Red);
            BOOST_CHECK(flag == true);

            file = "/home/bbrave/Desktop/OTUS/homeworks/hw11/input/test2.txt";
            flag = MapReduce(4, file, N_Map, N_Red);
            BOOST_CHECK(flag == false);
            flag = MapReduce(10, file, N_Map, N_Red);
            BOOST_CHECK(flag == true);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
