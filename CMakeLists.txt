cmake_minimum_required(VERSION 3.25)
project(tcp_chat)

set(CMAKE_CXX_STANDARD 20)

include_directories (${CURSES_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

find_package(GTest REQUIRED)
find_package(Curses REQUIRED)
find_package(Boost REQUIRED system thread timer chrono log_setup log)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -lgtest -lgmock -pthread -DNCURSES_NOMACROS")

add_executable (server_run server/src/main.cpp server/src/server.cpp server/src/server.h)
target_link_libraries (server_run ${Boost_LIBRARIES})

add_executable(test-server server/tests/test-server.cpp)
target_link_libraries (test-server ${GTEST_LIBRARIES})

add_executable (client_run client/src/main.cpp client/src/client.cpp client/src/client.h)
target_link_libraries(client_run ${CURSES_LIBRARIES} ${Boost_LIBRARIES})
enable_testing()

