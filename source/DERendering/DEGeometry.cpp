#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

#include <fstream>

void DvigEngine2::GeometryComponent::Init(const char* optGeometryDataPathOnDrive, const char* optIndicesDataPathOnDrive, void* optGeometryData, void* optIndicesData, deusize optGeometryDataByteWidth, deusize optIndicesDataByteWidth)
{
    DvigEngine2::Engine* engine = this->GetEngine();

    // Geometry data address
    void* geometryData = optGeometryData;
    void* indicesData = optIndicesData;
    deusize geometryDataByteWidth = optGeometryDataByteWidth;
    deusize indicesDataByteWidth = optIndicesDataByteWidth;
    if (geometryData == nullptr)
    {
        // Open files
        std::ifstream geometryFileStream( &optGeometryDataPathOnDrive[0], std::ios::binary );
        std::ifstream indicesFileStream( &optIndicesDataPathOnDrive[0], std::ios::binary );

        geometryFileStream.seekg(0, std::ios::end);
        deusize geometryDataFileByteWidth = geometryFileStream.tellg();
        geometryFileStream.seekg(0, std::ios::beg);

        indicesFileStream.seekg(0, std::ios::end);
        deusize indicesDataFileByteWidth = indicesFileStream.tellg();
        indicesFileStream.seekg(0, std::ios::beg);

        // Temporary memory objects
        DvigEngine2::MemoryObject* geometryTempMemoryObject = DvigEngine2::Engine::Allocate( 0, geometryDataFileByteWidth );
        geometryData = geometryTempMemoryObject->Unwrap<deuchar*>();
        geometryDataByteWidth = geometryDataFileByteWidth;
        DvigEngine2::MemoryObject* indicesTempMemoryObject = DvigEngine2::Engine::Allocate( 0, indicesDataFileByteWidth );
        indicesData = indicesTempMemoryObject->Unwrap<deuchar*>();
        indicesDataByteWidth = indicesDataFileByteWidth;

        // Read files
        geometryFileStream.read((char*)geometryData, geometryDataByteWidth);
        indicesFileStream.read((char*)indicesData, indicesDataByteWidth);

        // Delete temporary memory objects
        engine->Delete( geometryTempMemoryObject );
        engine->Delete( indicesTempMemoryObject );
    }

    // Copy to global buffers
    this->m_GeometryBufferByteWidth = geometryDataByteWidth;
    this->m_GeometryBufferOffset = DvigEngine2::RenderingSystem::m_GlobalGeometryBuffer->Insert( DV_NULL, geometryData, geometryDataByteWidth );
    this->m_IndexBufferByteWidth = indicesDataByteWidth;
    this->m_IndexBufferOffset = DvigEngine2::RenderingSystem::m_GlobalIndexBuffer->Insert( DV_NULL, indicesData, indicesDataByteWidth );

    // Update vertex buffer
    DvigEngine2::GL4::BindBuffer( GL_ARRAY_BUFFER, DvigEngine2::RenderingSystem::m_GLGlobalGeometryBuffer );
    DvigEngine2::GL4::BufferSubData( GL_ARRAY_BUFFER, this->m_GeometryBufferOffset, geometryDataByteWidth, geometryData );
    DvigEngine2::GL4::BindBuffer( GL_ARRAY_BUFFER, 0 );

    // Update index buffer
    DvigEngine2::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, DvigEngine2::RenderingSystem::m_GLGlobalIndexBuffer );
    DvigEngine2::GL4::BufferSubData( GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBufferOffset, indicesDataByteWidth, indicesData );
    DvigEngine2::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void DvigEngine2::GeometryComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}