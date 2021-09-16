cmake_minimum_required(VERSION 3.16)
project(squeak-game-engine)

# Language and compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-c -Wall)

#create all executables and libraries in the root of the build directory
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# GLFW
# Don't build docs, tests, or examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# Build GLFW
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/lib/glfw)
add_subdirectory(${GLFW_DIR} ${CMAKE_CURRENT_BINARY_DIR}/glfw)

#library must be built first
add_subdirectory(sge-lib)
add_subdirectory(test-app)
