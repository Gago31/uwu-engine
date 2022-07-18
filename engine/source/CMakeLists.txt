cmake_minimum_required(VERSION 3.20)
set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

project(UWUengine C CXX)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

cmake_path(GET CMAKE_SOURCE_DIR PARENT_PATH EXAMPLE_DIR)
cmake_path(GET EXAMPLE_DIR PARENT_PATH REPO_DIR)

add_library(uwu_lib STATIC
    "Grid.h" 
	"Grid.cpp"
    "Enemy.h"
    "Enemy.cpp" "Mesh.cpp" "Mesh.h" "Shader.h" "Shader.cpp" "Model.cpp" "Model.h" "Transform.h" "Sprite.h" "Sprite.cpp" "Map.h" "Map.cpp" "Renderer2D.h" "Texture.h" "Texture.cpp" "Renderer2D.cpp" "Transform.cpp" "ResourceManager.h" "ResourceManager.cpp" "InputController.h" "InputController.cpp" "Camera.h" "Camera.cpp" "Player.h" "Player.cpp" "Renderer.h" "Renderer.cpp" "GameController.h" "SceneGraph.h" "SceneGraph.cpp" "Node.h" "Node3D.h" "GridNode.h" "MeshNode.h" "GUINode.h" "TextRenderer.h" "ParticleEmitter.h" "Node2D.h" "SpriteNode.h" "TextBox.h" "SpriteNode.cpp" "NodeRegistry.h" "NodeRegistry.cpp" "TextRenderer.cpp" "Node.cpp" "SoundController.h" "SoundEmitter.h" "SoundEmitter2D.h" "SoundEmitter3D.h" "uwu.h")

include(GenerateExportHeader)
generate_export_header(uwu_lib)

add_executable(uwuEngine "main.cpp")

set_property(TARGET uwuEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET uwu_lib PROPERTY CXX_STANDARD 20)

set(INCLUDE_DIRS 
    "${REPO_DIR}/libs/grafica_cpp/third_party/glad/include/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/glfw-3.3.2/include/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/eigen-3.3.9/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/glm-0.9.9.8/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/stb/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/entt-v3.8.1/src/"
    "${REPO_DIR}/libs/grafica_cpp/third_party/assimp-5.0.1/include"
    "${REPO_DIR}/libs/grafica_cpp/third_party/openal-soft/include"
    "${REPO_DIR}/libs/grafica_cpp/third_party/dr_wav"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/assimp-5.0.1/include"
    "${REPO_DIR}/libs/grafica_cpp/third_party/stb/"
    "${REPO_DIR}/libs/freetype/include"
    "${REPO_DIR}/libs/json/single_include"
)

set(LIB_DIRS
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/glad/Release/glad.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/glfw-3.3.2/src/Release/glfw3.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/assimp-5.0.1/code/Release/assimp-vc143-mt.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/assimp-5.0.1/contrib/zlib/Release/zlibstatic.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/assimp-5.0.1/contrib/irrXML/Release/IrrXML.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/openal-soft/Release/OpenAL32.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/stb/Release/stb.lib"
    "${REPO_DIR}/libs/grafica_cpp/build/third_party/dr_wav/Release/dr_wav.lib"
    "${REPO_DIR}/libs/freetype/release dll/win64/freetype.lib"
)

target_include_directories(uwu_lib PRIVATE ${INCLUDE_DIRS})

target_link_libraries(uwu_lib PRIVATE ${LIB_DIRS})

target_link_libraries(uwuEngine PRIVATE
    ${LIB_DIRS}
    uwu_lib
)

target_include_directories(uwuEngine PRIVATE
    ${INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(CMakeConfigFiles/root_directory.h.in "${CMAKE_SOURCE_DIR}/root_directory.h")
