cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER             "/usr/bin/clang")
set(CMAKE_CXX_COMPILER           "/usr/bin/clang++")

add_definitions(-DCMAKE_BUILD_TYPE=Debug)
project(TopDownParserGen)

SET(CMAKE_CXX_FLAGS "-ggdb")

find_package(Boost REQUIRED)

SET(parser_gen_SRCS parser_gen.hpp parser_gen.cpp first.hpp utils.hpp common.hpp follow.hpp rule_action.hpp parenth.hpp prev.hpp calc.hpp)
add_executable(parser_gen ${parser_gen_SRCS})
