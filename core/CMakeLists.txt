cmake_minimum_required(VERSION 3.0.0)
project(TEngine)

set(TEngine_CORE_SOURCE

	"include/tengine/mgl.hpp"
	"include/tengine/mglApp.hpp"
	"include/tengine/mglCamera.hpp"
	"include/tengine/mglConventions.hpp"
	"include/tengine/mglError.hpp"
	"include/tengine/mglShader.hpp"
)

set(TEngine_CORE_INCLUDE

	"src/tengine/mglApp.cpp"
	"src/tengine/mglCamera.cpp"
	"src/tengine/mglError.cpp"
	"src/tengine/mglShader.cpp"
)


include_directories("libs/glm")
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_library(tengine-core ${TEngine_CORE_SOURCE} ${TEngine_CORE_INCLUDE})
target_include_directories(tengine-core PUBLIC "include" PRIVATE "src")
set_property(TARGET tengine-core PROPERTY CXX_STANDARD 20)
target_compile_features(tengine-core PUBLIC cxx_std_20)
target_link_libraries(tengine-core 
	PUBLIC glm
	PUBLIC glm
    PUBLIC GLEW
	PUBLIC glfw
	PUBLIC GL
)