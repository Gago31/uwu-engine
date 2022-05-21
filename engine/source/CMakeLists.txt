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
    "Enemy.h" "Enemy.cpp")

include(GenerateExportHeader)
generate_export_header(uwu_lib)


add_executable(uwuEngine "main.cpp")

set_property(TARGET uwuEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET uwu_lib PROPERTY CXX_STANDARD 20)

target_include_directories(uwu_lib PRIVATE
    "${REPO_DIR}/libs/grafica_cpp/source/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/glad/include/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/glfw-3.3.2/include/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/eigen-3.3.9/"
)

target_link_libraries(uwu_lib PRIVATE
    "${REPO_DIR}/libs/grafica_cpp/build/source/grafica/Release/grafica.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/glad/Release/glad.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/glfw-3.3.2/src/Release/glfw3.lib"
)

target_link_libraries(uwuEngine PRIVATE
    "${REPO_DIR}/libs/grafica_cpp/build/source/grafica/Release/grafica.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/glad/Release/glad.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/glfw-3.3.2/src/Release/glfw3.lib"
    uwu_lib
)

target_include_directories(uwuEngine PRIVATE
    "${REPO_DIR}/libs/grafica_cpp/source/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/glad/include/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/glfw-3.3.2/include/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/eigen-3.3.9/"
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(CMakeConfigFiles/root_directory.h.in "${CMAKE_SOURCE_DIR}/root_directory.h")
