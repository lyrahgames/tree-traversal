cmake_minimum_required(VERSION 3.10)
project(binary-tree-iterator VERSION 0.1.0 LANGUAGES CXX)
add_executable(main main.cc)
target_compile_features(main PRIVATE cxx_std_17)

add_executable(test test.cc)
# target_compile_features(test PRIVATE cxx_std_17)