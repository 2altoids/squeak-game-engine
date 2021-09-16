# Copy resource files into build
# Copy shaders
file(GLOB SHADERS ${PROJECT_SOURCE_DIR}/res/shaders/*.glsl)
file(COPY ${SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../res/shaders)

add_executable(test src/main.cpp)

target_link_libraries(test PRIVATE sge-lib)
