#sge-lib
# Add source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# Add header files
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_library(
  sge-lib SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  #build glad inside sge-lib
  ${PROJECT_SOURCE_DIR}/lib/glad/include/glad/glad.h
  ${PROJECT_SOURCE_DIR}/lib/glad/include/KHR/khrplatform.h
  ${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c
)

target_link_libraries(sge-lib glfw ${GLFW_LIBRARIES}) #add glfw to the sge-lib library

# Add directory sge-lib/src/ to the include path of the sge-lib target (the library this file is responsible for building).  This allows test-app to include sge-lib's own header files.
# PUBLIC means any other target using this library needs to have the library's include path added to it.
# CMAKE_CURRENT_SOURCE_DIR refers to the folder containing the currently-being-processed CMakeLists (sge-lib, in this example)
target_include_directories(sge-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
#append glad's include directory to the include path as well
target_include_directories(sge-lib PUBLIC "${PROJECT_SOURCE_DIR}/lib/glad/include")
