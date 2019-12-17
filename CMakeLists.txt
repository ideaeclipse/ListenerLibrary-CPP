cmake_minimum_required(VERSION 3.12)
project(ListenerLibrary_CPP)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(ListenerLibrary_CPP main.cpp)

target_link_libraries(ListenerLibrary_CPP Threads::Threads)
