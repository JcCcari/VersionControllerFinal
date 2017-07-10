cmake_minimum_required(VERSION 3.7)
project(VersionController)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp graph.cpp graph.h nodeAndEdge.h tools.h CFile.h hashTable.h CPersistence.h)
add_executable(VersionController ${SOURCE_FILES})