project(TEngine)

set(TEngine_CORE_SOURCE

	"src/tengine/core/test.cpp"
)

set(TEngine_CORE_INCLUDE

	"include/tengine/core/test.hpp"
)


add_library(tengine-core ${TEngine_CORE_SOURCE} ${TEngine_CORE_INCLUDE})
target_include_directories(tengine-core PUBLIC "include" PRIVATE "src")