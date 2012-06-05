cmake_minimum_required(VERSION 2.8)
add_definitions(-DCMAKE_BUILD_TYPE=Debug)
project(TopDownParserGen)

find_package(Boost REQUIRED)

SET(parser_gen_SRCS grammar.h grammar.cpp parser_gen.h parser_gen.cpp)
add_executable(parser_gen ${parser_gen_SRCS})
