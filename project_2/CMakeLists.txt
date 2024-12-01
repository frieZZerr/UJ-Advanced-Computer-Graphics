cmake_minimum_required(VERSION 3.10)
project(CannonBall)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgViewer osgUtil osg)

set(SOURCES
    src/Main.cpp
    src/input/InputHandler.cpp
    src/scene/Scene.cpp
    src/scene/objects/Projectile.cpp
    src/scene/objects/Grass.cpp
    src/scene/objects/Cannon.cpp
    src/scene/objects/Target.cpp
)

add_executable(game ${SOURCES})
target_link_libraries(game osgDB osgGA osgViewer osgUtil osg)
