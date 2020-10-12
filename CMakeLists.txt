cmake_minimum_required(VERSION 3.0.0)
project(conways VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -O3 -pedantic")

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR lib/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics)

include_directories(inc)

add_executable(conways src/main.cpp src/Cell.cpp)

target_link_libraries(conways sfml-graphics)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)