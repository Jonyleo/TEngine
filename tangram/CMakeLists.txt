cmake_minimum_required(VERSION 3.0.0)
project(tangram)

add_executable(${PROJECT_NAME} hello-2d-world.cpp)

include_directories("libs/glm")
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PUBLIC tengine-core
    PUBLIC glm
    PUBLIC GLEW
	PUBLIC glfw
	PUBLIC GL
)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/assets)
file(COPY        ${CMAKE_CURRENT_SOURCE_DIR}/assets 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)