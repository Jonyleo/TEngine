cmake_minimum_required(VERSION 3.0.0)
project(tangram)

add_executable(${PROJECT_NAME} tangram.cpp)

include_directories("libs/glm/glm")

target_link_libraries(${PROJECT_NAME}
    PUBLIC tengine-core
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)