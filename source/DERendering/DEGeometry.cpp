#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

#include <fstream>

DvigEngine2::DynamicBuffer* DvigEngine2::GeometryComponent::m_GlobalGeometryBuffer = nullptr;
DvigEngine2::DynamicBuffer* DvigEngine2::GeometryComponent::m_GlobalIndexBuffer = nullptr;

void DvigEngine2::GeometryComponent::Init(const char* optGeometryDataPathOnDrive, const char* optIndicesDataPathOnDrive, void* optBufferData, deusize optGeometryDataByteWidth, deusize optIndicesDataByteWidth)
{
    DvigEngine2::Engine* engine = this->GetEngine();

    // Create global geometry buffer
    // If needed
    if (DvigEngine2::GeometryComponent::m_GlobalGeometryBuffer == nullptr)
    {
        engine->Create<DvigEngine2::DynamicBuffer>( &GeometryComponent::m_GlobalGeometryBuffer, "_GlobalGeometryBuffer" );
        GeometryComponent::m_GlobalGeometryBuffer->Init( 0, 1024 );
        engine->Create<DvigEngine2::DynamicBuffer>( &GeometryComponent::m_GlobalIndexBuffer, "_GlobalIndexBuffer" );
        GeometryComponent::m_GlobalIndexBuffer->Init( 0, 1024 );
    }

    // Geometry data address
    void* geometryData = optBufferData;
    void* indicesData;
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
    else
    {
        indicesData = DvigEngine2::Ptr<void*>::Add( &optBufferData, optGeometryDataByteWidth );
    }

    // Copy to global buffers
    this->m_GeometryBufferByteWidth = geometryDataByteWidth;
    this->m_GeometryBufferOffset = DvigEngine2::GeometryComponent::m_GlobalGeometryBuffer->Insert( DV_NULL, geometryData, geometryDataByteWidth );
    this->m_IndexBufferByteWidth = indicesDataByteWidth;
    this->m_IndexBufferOffset = DvigEngine2::GeometryComponent::m_GlobalIndexBuffer->Insert( DV_NULL, indicesData, indicesDataByteWidth );

    // Bind vertex buffer here
}

void DvigEngine2::GeometryComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}

void DvigEngine2::GeometryComponent::ClearGeometryBuffer()
{
    DV_ASSERT_PTR(DvigEngine2::GeometryComponent::m_GlobalGeometryBuffer)

    DvigEngine2::GeometryComponent::m_GlobalGeometryBuffer->Free();
    DvigEngine2::GeometryComponent::m_GlobalGeometryBuffer = nullptr;
}