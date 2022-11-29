cmake_minimum_required(VERSION 3.0.0)
project(TEngine)

set(TEngine_CORE_SOURCE

	"src/tengine/test.cpp"
)

set(TEngine_CORE_INCLUDE

	"include/tengine/test.hpp"
)


include_directories("libs/glm/glm")

add_library(tengine-core ${TEngine_CORE_SOURCE} ${TEngine_CORE_INCLUDE})
target_include_directories(tengine-core PUBLIC "include" PRIVATE "src")
set_property(TARGET tengine-core PROPERTY CXX_STANDARD 20)
target_compile_features(tengine-core PUBLIC cxx_std_20)