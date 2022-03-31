cmake_minimum_required(VERSION 3.20)
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

project(UWUengine C CXX)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

cmake_path(GET CMAKE_SOURCE_DIR PARENT_PATH EXAMPLE_DIR)
cmake_path(GET EXAMPLE_DIR PARENT_PATH REPO_DIR)

add_library(uwu_lib SHARED
    "Grid.h" 
	"Grid.cpp"
)

include(GenerateExportHeader)
generate_export_header(uwu_lib)

add_executable(uwuEngine "main.cpp")

set_property(TARGET uwuEngine PROPERTY CXX_STANDARD 20)

target_link_libraries(uwuEngine PRIVATE
    "${REPO_DIR}/libs/glad/glad.lib"
    "${REPO_DIR}/libs/glfw/glfw3.lib"
    "${REPO_DIR}/libs/grafica/grafica.lib"
    uwu_lib
)
target_include_directories(uwuEngine PRIVATE
    "${REPO_DIR}/libs/glad/include"
    "${REPO_DIR}/libs/glfw/include"
    "${REPO_DIR}/libs/grafica/include"
    ${CMAKE_CURRENT_BINARY_DIR}
)


