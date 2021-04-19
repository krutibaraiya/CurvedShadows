cmake_minimum_required(VERSION 3.16)
project(CurvedShadows)
find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(CurvedShadows main.cpp Shader.cpp Shader.h ObjModel.cpp ObjModel.h Camera.cpp Camera.h Parser.cpp Parser.h Light.cpp Light.h)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
include_directories(CurvedShadows GLM_INCLUDE_DIR)

find_package(ASSIMP REQUIRED)
include_directories(CurvedShadows ASSIMP_INCLUDE_DIRS)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(CurvedShadows GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor assimp GLEW::GLEW)