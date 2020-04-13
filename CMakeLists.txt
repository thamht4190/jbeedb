cmake_minimum_required(VERSION 3.10)

project(jbeedb)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

set(SRC
    src/Record.cpp
    src/Store.cpp
    src/Variant.cpp
)

add_library(jbeedb STATIC ${SRC})

# test
add_subdirectory(vendors/googletest/googletest)

add_executable(StoreTest test/StoreTest.cpp)
target_include_directories(StoreTest PRIVATE vendors/googletest/googletest/include)
target_link_libraries(StoreTest PRIVATE jbeedb gtest)