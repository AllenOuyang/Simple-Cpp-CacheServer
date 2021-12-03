cmake_minimum_required(VERSION 3.0)

project(CACHECLIENT)

include_directories(include)

add_compile_options(-Wall -O2)

add_executable(client client.cpp src/TcpServer.cpp src/TcpSocket.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(client PRIVATE Threads::Threads)
