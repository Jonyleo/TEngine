project(tangram)

add_executable(${PROJECT_NAME} tangram.cpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC tengine-core
)