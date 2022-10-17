cmake_minimum_required(VERSION 3.18)

project(instruction_generator VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

set(FCOMPUTER_MODULES "")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/instruction_set_generator)

add_executable(fcomputer "")
target_sources(fcomputer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

if (FCOMPUTER_MODULES)
    target_link_libraries(fcomputer PRIVATE ${FCOMPUTER_MODULES})
endif (FCOMPUTER_MODULES)
