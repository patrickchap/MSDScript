

#version of CMake
cmake_minimum_required(VERSION 3.15)

#project name
project(MSDscript)
project(MSDLib)

#create a static library 
ADD_LIBRARY(MSDLib STATIC  src/catch.hpp src/cont.cpp src/Env.cpp src/expr.cpp  src/parser.cpp src/step.cpp src/value.cpp  ../test/run.cpp)



#exacutable for command line MSDscript
add_executable(MSDscript src/catch.hpp src/cont.cpp src/Env.cpp src/expr.cpp src/main.cpp src/parser.cpp src/step.cpp src/value.cpp  ../test/run.cpp)


link_directories(/Users/patrickchapple/PatrickChapple/CS6015/hw01/hw01/build)


#comile options
target_compile_options(MSDscript PUBLIC -std=c++11)
target_compile_options(MSDLib PUBLIC -std=c++11)

