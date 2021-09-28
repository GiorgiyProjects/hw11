cmake_minimum_required(VERSION 3.5)

project(mapreduce)

find_package(Boost 1.65.0 COMPONENTS system unit_test_framework REQUIRED)
add_executable(mapreduce main.cpp Mapper.h Shuffler.h Reducer.h InputFairDivider.h MapReduce.h)
add_executable(test tests.cpp Mapper.h Shuffler.h Reducer.h InputFairDivider.h MapReduce.h)

set(TARGETS mapreduce test)

foreach(target ${TARGETS})
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
            CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PRIVATE "-g;-Wall;-Wextra;-Werror;-pedantic;-lstdc++fs")
        target_link_libraries(${target} PUBLIC pthread)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE "/W4;/WX")
    endif()
    target_compile_features(${target} PUBLIC cxx_std_17)
endforeach(target IN ${TARGETS})
