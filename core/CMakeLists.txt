cmake_minimum_required(VERSION 3.0.0)
project(TEngine)

if(MSVC)
	find_library(glfw3 REQUIRED
				NAMES glfw3
				HINTS "${CMAKE_SOURCE_DIR}/dependencies/lib"
	)
	find_library(GLEW REQUIRED
				NAMES glew32
				HINTS "${CMAKE_SOURCE_DIR}/dependencies/lib"
	)

elseif(UNIX AND NOT APPLE) 
	find_package(glfw3 REQUIRED)
	find_package(GLEW REQUIRED)
endif()

set(TEngine_CORE_INCLUDE

	"include/tengine/mgl.hpp"
	"include/tengine/mglApp.hpp"
	"include/tengine/mglCamera.hpp"
	"include/tengine/mglConventions.hpp"
	"include/tengine/mglError.hpp"
	"include/tengine/mglShader.hpp"

	"include/tengine/DataStructs.hpp"
	"include/tengine/Component.hpp"

	"include/tengine/Mesh.hpp"
	"include/tengine/Entity.hpp"
	"include/tengine/Transform.hpp"
	"include/tengine/MeshInstance.hpp"
	"include/tengine/ShaderManager.hpp"
	"include/tengine/Scene.hpp"
	"include/tengine/Resources.hpp"
)

set(TEngine_CORE_SOURCE

	"src/tengine/mglApp.cpp"
	"src/tengine/mglCamera.cpp"
	"src/tengine/mglError.cpp"
	"src/tengine/mglShader.cpp"

	"src/tengine/Mesh.cpp"
	"src/tengine/Entity.cpp"
	"src/tengine/Transform.cpp"
	"src/tengine/MeshInstance.cpp"
	"src/tengine/ShaderManager.cpp"
	"src/tengine/Scene.cpp"
	
)

add_library(tengine-core ${TEngine_CORE_SOURCE} ${TEngine_CORE_INCLUDE})
target_include_directories(tengine-core 
	PUBLIC "include" 
	PRIVATE "src"
)

target_include_directories(tengine-core 
	PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/include"
)

if(MSVC) 
	target_link_libraries(tengine-core
		PUBLIC ${glfw3}
		PUBLIC ${GLEW}
		PUBLIC opengl32
	)
elseif(UNIX AND NOT APPLE) 
	target_link_libraries(tengine-core
		PUBLIC glfw
		PUBLIC GLEW
		PUBLIC GL
	)
endif()

set_property(TARGET tengine-core PROPERTY CXX_STANDARD 20)
target_compile_features(tengine-core PUBLIC cxx_std_20)