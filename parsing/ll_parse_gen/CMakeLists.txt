cmake_minimum_required(VERSION 2.8)
add_definitions(-DCMAKE_BUILD_TYPE=Debug)
project(TopDownParserGen)

SET(CMAKE_CXX_FLAGS "-ggdb")

find_package(Boost REQUIRED)

SET(parser_gen_SRCS parser_gen.hpp parser_gen.cpp first.hpp utils.hpp common.hpp follow.hpp rule_action.hpp)
add_executable(parser_gen ${parser_gen_SRCS})
