cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

set(INSTALL_FOLDER ${CMAKE_CURRENT_LIST_DIR}/install/${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_DEBUG_MACRO")

add_subdirectory(LagEngine)
add_subdirectory(TestApplication)