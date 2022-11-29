cmake_minimum_required(VERSION 3.0.0)
project(tangram)

add_executable(${PROJECT_NAME} tangram.cpp)

find_package(glm REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PUBLIC tengine-core
    PUBLIC glm::glm
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)