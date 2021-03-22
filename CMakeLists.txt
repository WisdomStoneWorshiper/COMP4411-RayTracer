cmake_minimum_required(VERSION 3.0)

project(PROJ3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_EXTENSIONS ON)

find_package(OpenGL REQUIRED)
find_package(FLTK REQUIRED)

add_executable(proj3)

target_include_directories(proj3 PUBLIC ./)
#target_include_directories(proj3 PUBLIC ./fltk-1.3.3/)

target_link_libraries(proj3 PRIVATE ${OPENGL_LIBRARIES})

include_directories(${FLTK_INCLUDE_DIR})
target_link_libraries(proj3 PRIVATE ${FLTK_LIBRARIES})

target_sources(proj3 PRIVATE 
src/RayTracer.cpp   
src/getopt.cpp      
src/main.cpp
src/SceneObjects/Box.cpp         
src/SceneObjects/Cone.cpp        
src/SceneObjects/Cylinder.cpp    
src/SceneObjects/Sphere.cpp      
src/SceneObjects/Square.cpp      
src/SceneObjects/trimesh.cpp
src/fileio/bitmap.cpp      
src/fileio/parse.cpp       
src/fileio/read.cpp
src/scene/camera.cpp      
src/scene/light.cpp       
src/scene/material.cpp    
src/scene/ray.cpp         
src/scene/scene.cpp
src/ui/TraceGLWindow.cpp       
src/ui/TraceUI.cpp
src/vecmath/vecmath.cpp)