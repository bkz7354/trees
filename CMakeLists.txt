cmake_minimum_required(VERSION 3.2)

project(Trees)

add_executable(tree_test tree_test.cpp)

set_target_properties(tree_test PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(test1 tree_test)