cmake_minimum_required(VERSION 3.26)
project(shortest_path LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main executable
add_executable(shortest_path
        main.cpp
        graph.cpp
        vertex.cpp
)

# Compiler options
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    target_compile_options(shortest_path PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic)
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(shortest_path PRIVATE /W4)
endif()

# Setting up threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(shortest_path Threads::Threads)

# Set the startup project for Visual Studio
if (${CMAKE_VERSION} VERSION_GREATER 3.5)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT shortest_path)
endif()
