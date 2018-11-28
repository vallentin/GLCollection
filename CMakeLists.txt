cmake_minimum_required(VERSION 3.12)
project(GLCollection)

set(CMAKE_CXX_STANDARD 11)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw)

include_directories(${PROJECT_SOURCE_DIR}/libs/glad/include)
file(GLOB_RECURSE GLAD ${PROJECT_SOURCE_DIR}/libs/glad/*.h* ${PROJECT_SOURCE_DIR}/libs/glad/*.c*)
