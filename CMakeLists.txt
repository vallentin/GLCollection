cmake_minimum_required(VERSION 3.12)
project(GLCollection)

set(CMAKE_CXX_STANDARD 11)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw)

include_directories(${PROJECT_SOURCE_DIR}/libs)
include_directories(${PROJECT_SOURCE_DIR}/libs/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/stb)

file(GLOB_RECURSE GLAD ${PROJECT_SOURCE_DIR}/libs/glad/*.h* ${PROJECT_SOURCE_DIR}/libs/glad/*.c*)

add_executable(cube cube.cpp ${GLAD})
target_link_libraries(cube glfw)

add_executable(screen_quad screen_quad.cpp ${GLAD})
target_link_libraries(screen_quad glfw)

add_executable(screenshot screenshot.cpp ${GLAD})
target_link_libraries(screenshot glfw)
