cmake_minimum_required(VERSION 3.10)

set(APP_NAME TestApplication)

project(${APP_NAME})
set(CMAKE_CXX_STANDARD 11)

set(SRC_FOLDER src)

set(SOURCES
        ${SRC_FOLDER}/main.cpp
        ${SRC_FOLDER}/TestApplication.cpp
        ${SRC_FOLDER}/TestScene.cpp
        ${SRC_FOLDER}/TestScene2.cpp)

set(INCLUDE_FOLDERS ${SRC_FOLDER})

add_executable(${APP_NAME} ${SOURCES})

target_include_directories(${APP_NAME} PRIVATE ${INCLUDE_FOLDERS})

target_link_libraries(${APP_NAME} LagEngine)


install(TARGETS ${APP_NAME} RUNTIME DESTINATION ${INSTALL_FOLDER})
install(DIRECTORY resources DESTINATION ${INSTALL_FOLDER})
install(FILES startupDebug.ini DESTINATION ${INSTALL_FOLDER} RENAME startup.ini CONFIGURATIONS debug)
install(FILES startupRelease.ini DESTINATION ${INSTALL_FOLDER} RENAME startup.ini CONFIGURATIONS release)