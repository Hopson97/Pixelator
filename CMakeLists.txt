cmake_minimum_required(VERSION 3.1)

project(pixelator)
set (CMAKE_CXX_STANDARD 17)

#set (CMAKE_CXX_STANDARD_LIBRARIES -lstdc++fs)
#fs unused for now

#Find the SFML package
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML REQUIRED audio network graphics window system)

#Include directory
include_directories(src/Dependencies/)

#Add files
set(SOURCES 
    src/Main.cpp
)



set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


#Define the executable
add_executable(
    ${PROJECT_NAME} 
    ${SOURCES} 
)

SET(ADDITIONAL_LINK_FLAGS)
if(UNIX)
    list(APPEND ADDITIONAL_LINK_FLAGS dl)
endif(UNIX)

find_package(Threads)
target_link_libraries(${PROJECT_NAME} Threads::Threads ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${ADDITIONAL_LINK_FLAGS})