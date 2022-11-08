cmake_minimum_required(VERSION 3.18)

project(fcomputer VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(fcomputer "")
target_include_directories(fcomputer PRIVATE
        ${ANTLR4_INCLUDE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/src/languages/generated"
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/antlr4.11.1/antlr4-runtime")
target_link_directories(fcomputer PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/antlr4.11.1/lib")
target_sources(fcomputer PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/languages/generated/FComputerASMListener.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/languages/generated/FComputerASMLexer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/languages/generated/FComputerASMBaseListener.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/languages/generated/FComputerASMParser.cpp
        )

target_link_libraries(fcomputer PRIVATE antlr4-runtime.a)