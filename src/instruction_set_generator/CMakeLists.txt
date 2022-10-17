cmake_minimum_required(VERSION 3.1)
project("instruction_set_generator" VERSION 0.1)
set(CMAKE_CXX_STANDARD 14)

add_library(instruction_set_generator STATIC "")

target_sources(instruction_set_generator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/instruction_set_generator.cpp)
target_include_directories(instruction_set_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(FCOMPUTER_MODULES ${FCOMPUTER_MODULES} "instruction_set_generator" PARENT_SCOPE)
