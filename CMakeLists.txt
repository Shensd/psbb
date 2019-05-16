cmake_minimum_required(VERSION 3.1.0)

project(PSBB)

include_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(PSBB ${SOURCES})

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(PSBB Threads::Threads)