cmake_minimum_required(VERSION 3.2)

project(DvigEngine)

set(CMAKE_CXX_STANDARD 11)

if (MSVC)

else()
    add_compile_options(-O3)
endif()

add_library(DvigEngine_v10_00 STATIC
	Source/Application/Application.cpp
	Source/Core/Collection.cpp
	Source/Core/Engine.cpp
	Source/Core/Debug.cpp
	Source/Core/HashedCollection.cpp
	Source/Core/ISystem.cpp
	Source/Core/Node.cpp
	Source/Core/StringSystem.cpp
	Source/GUI/IWindow.cpp
	Source/Rendering/D3D11.cpp
	Source/Rendering/Geometry.cpp
	Source/Rendering/RenderingSystem.cpp
	Source/Rendering/RenderTargetGroup.cpp
	Source/Rendering/Shader.cpp
	Source/Rendering/Transform.cpp
	Source/Rendering/Viewer.cpp
)

#include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/XAudio2/include)
link_directories(${CMAKE_SOURCE_DIR}/ThirdParty/glfw/lib)
#link_directories(${CMAKE_SOURCE_DIR}/ThirdParty/XAudio2/lib)

if (MSVC)
	target_link_libraries(DvigEngine_v10_00 d3d11.lib)
	#target_link_libraries(DvigEngine_v10_00 d3dcompiler.lib)
	target_link_libraries(DvigEngine_v10_00 glfw3.lib)
endif()

add_executable(DvigEngine
    Example/main.cpp
)

target_link_libraries(DvigEngine DvigEngine_v10_00)