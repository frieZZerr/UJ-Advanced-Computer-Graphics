cmake_minimum_required(VERSION 3.10)
project(CannonBall)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgViewer osgUtil osg)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(game ${SOURCES})

target_link_libraries(game osg osgDB osgGA osgViewer osgUtil)
target_include_directories(game PRIVATE ${CMAKE_SOURCE_DIR}/src)

message(STATUS "Found source files: ${SOURCES}")
