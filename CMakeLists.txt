cmake_minimum_required(VERSION 3.2)

project(DvigEngine)

set(CMAKE_CXX_STANDARD 11)

if (MSVC)

else()
    add_compile_options(-O3)
endif()

add_library(DvigEngine_v10_00 STATIC
	source/DEApplication/DEApplication.cpp
	source/DECore/DECollection.cpp
	source/DECore/DEEngine.cpp
	source/DECore/DEDebug.cpp
	source/DECore/DEHashedCollection.cpp
	source/DECore/DEINode.cpp
	source/DECore/DEStringSystem.cpp
	source/DEGUI/DEIWindow.cpp
	source/DERendering/DED3D11.cpp
	source/DERendering/DEGeometry.cpp
	source/DERendering/DERenderingSystem.cpp
	source/DERendering/DERenderTargetGroup.cpp
	source/DERendering/DEShader.cpp
	source/DERendering/DETransform.cpp
	source/DERendering/DEViewer.cpp
)

#include_directories(${CMAKE_SOURCE_DIR}/thirdparty/XAudio2/include)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/glfw/lib)
#link_directories(${CMAKE_SOURCE_DIR}/thirdparty/XAudio2/lib)

if (MSVC)
	target_link_libraries(DvigEngine_v10_00 d3d11.lib)
	#target_link_libraries(DvigEngine_v10_00 d3dcompiler.lib)
	target_link_libraries(DvigEngine_v10_00 glfw3.lib)
endif()

add_executable(DvigEngine
    example/main.cpp
)

target_link_libraries(DvigEngine DvigEngine_v10_00)