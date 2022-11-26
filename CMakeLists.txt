cmake_minimum_required(VERSION 3.2)

project(DvigEngine)

set(CMAKE_CXX_STANDARD 11)

if (MSVC)
    add_compile_options(/O2)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -O3)
endif()

add_library(DvigEngine_v10_00 STATIC
    source/DEApplication/DEApplication.cpp
    source/DEAudio/DEAudioSource.cpp
    source/DEAudio/DEAudioSystem.cpp
    source/DECore/DEDynamicBuffer.cpp
    source/DECore/DEEngine.cpp
    source/DECore/DEFixedSet.cpp
    source/DECore/DEHashMap.cpp
    source/DECore/DEICommon.cpp
    source/DECore/DEINode.cpp
    source/DECore/DERegistry.cpp
    source/DECore/DEString.cpp
    source/DECore/DEThreadPoolSystem.cpp
    source/DEGUI/DEIWindow.cpp
    source/DERendering/DEGeometry.cpp
    source/DERendering/DEMaterial.cpp
    source/DERendering/DEOpenGL4.cpp
    source/DERendering/DEPostProcessor.cpp
    source/DERendering/DERenderingSystem.cpp
    source/DERendering/DERenderTargetGroup.cpp
    source/DERendering/DEShader.cpp
    source/DERendering/DETextureMergerSystem.cpp
    source/DERendering/DETransform.cpp
    source/DERendering/DEViewer.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/XAudio2/include)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/glfw/lib)
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/XAudio2/lib)

target_link_libraries(DvigEngine_v10_00 libglfw3.a)

add_executable(DvigEngine
    source/main.cpp
)

target_link_libraries(DvigEngine DvigEngine_v10_00)