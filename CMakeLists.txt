cmake_minimum_required(VERSION 3.18)

project(fcomputer VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
if (BISON_FOUND)
    set(BisonOutput "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c")
    add_custom_command(
            OUTPUT ${BisonOutput}
            COMMAND ${BISON_EXECUTABLE}
            ARGS -do ${BisonOutput} ${CMAKE_CURRENT_SOURCE_DIR}/parser/fasm.y
            MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/parser/fasm.y)
endif (BISON_FOUND)

if (FLEX_FOUND)
    set(FlexOutput "${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c")
    add_custom_command(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            ARGS -o ${FlexOutput} ${CMAKE_CURRENT_SOURCE_DIR}/parser/fasm.l
            MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/parser/fasm.l)
endif (FLEX_FOUND)

add_library(yacc_lib ${BisonOutput})
target_include_directories(yacc_lib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
        ${CMAKE_CURRENT_BINARY_DIR}
        )

add_executable(parser "")
target_include_directories(parser
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(parser
        PRIVATE
        yacc_lib)

target_sources(parser
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/emit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/program.c
        ${BisonOutput}
        ${FlexOutput}
        #${CMAKE_CURRENT_BINARY_DIR}/parser/lex.yy.c
        #${CMAKE_CURRENT_BINARY_DIR}/parser/y.tab.c
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/pass/pass_update_label.c
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/pass/pass_reduce_program.c
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/optimize.c)
