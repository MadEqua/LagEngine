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


###############################################
#Install
###############################################
install(TARGETS ${APP_NAME} RUNTIME DESTINATION ${INSTALL_FOLDER})
install(FILES startupDebug.ini DESTINATION ${INSTALL_FOLDER} RENAME startup.ini CONFIGURATIONS debug)
install(FILES startupRelease.ini DESTINATION ${INSTALL_FOLDER} RENAME startup.ini CONFIGURATIONS release)

#On Release builds we copy the resources
install(DIRECTORY resources DESTINATION ${INSTALL_FOLDER} CONFIGURATIONS release)

#On Debug builds we create a symlink. Useful for editing the original files and hot reload them. (not working on Windows)
#install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink resources ${INSTALL_FOLDER}/resources)" CONFIGURATIONS debug)