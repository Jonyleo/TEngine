cmake_minimum_required(VERSION 3.0.0)
project(tangram)


set(MY_GAME_INCLUDE

	"tangram.hpp"
	"door.hpp"
	"camera.hpp"
	"save.hpp"
	"common.hpp"
)

set(MY_GAME_SOURCE

	"tangram.cpp"
	"main.cpp"
)



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

add_executable(${PROJECT_NAME} ${MY_GAME_INCLUDE} ${MY_GAME_SOURCE})

target_include_directories(${PROJECT_NAME} 
	PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/include"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC tengine-core
)

if(MSVC) 
	target_link_libraries(${PROJECT_NAME}
		PUBLIC ${glfw3}
		PUBLIC ${GLEW}
		PUBLIC opengl32
	)
elseif(UNIX AND NOT APPLE) 
	target_link_libraries(${PROJECT_NAME}
		PUBLIC glfw
		PUBLIC GLEW
		PUBLIC GL
	)
endif()

if(MSVC) 
	file(COPY ${CMAKE_SOURCE_DIR}/dependencies/lib/glew32.dll
		DESTINATION  ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${CMAKE_SOURCE_DIR}/dependencies/lib/assimp-vc143-mtd.dll
		DESTINATION  ${CMAKE_CURRENT_BINARY_DIR})
endif()

set(TANGRAM_RESOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/assets
)

add_custom_target(copy_files ALL)

add_custom_command(TARGET copy_files
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${TANGRAM_RESOURCES}
	${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_custom_command(TARGET copy_files
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory
	${CMAKE_CURRENT_BINARY_DIR}/saves
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)