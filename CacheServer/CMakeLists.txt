cmake_minimum_required(VERSION 3.0)

project(CACHESERVER)

include_directories(include)

add_compile_options(-Wall -O2)

add_executable(cacheserver cacheserver.cpp src/find.cpp src/lru.cpp src/TcpServer.cpp src/TcpSocket.cpp src/CacheServer.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cacheserver PRIVATE Threads::Threads)
