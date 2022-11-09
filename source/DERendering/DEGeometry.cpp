#include "../../include/DECore.hpp"
#include "../../include/DERendering.hpp"

#include <fstream>

DvigEngine2::DynamicBuffer* DvigEngine2::GeometryBatch::m_GlobalGeometryBuffer = nullptr;

void DvigEngine2::GeometryComponent::Init(const char* optGeometryPathOnDrive, void* optGeometryData, deusize optGeometryDataByteWidth)
{
    DvigEngine2::Engine* engine = this->GetEngine();

    // Create global geometry buffer
    // If needed
    if (DvigEngine2::GeometryBatch::m_GlobalGeometryBuffer == nullptr)
    {
        engine->Create<DvigEngine2::DynamicBuffer>( &GeometryBatch::m_GlobalGeometryBuffer, "_GlobalGeometryBuffer" );
        GeometryBatch::m_GlobalGeometryBuffer->Init( 0, 1024 );
    }

    // Geometry data address
    void* geometryData = optGeometryData;
    deusize geometryDataByteWidth = optGeometryDataByteWidth;
    if (geometryData == nullptr)
    {
        // Open file
        std::ifstream fileStream( &optGeometryPathOnDrive[0], std::ios::binary );

        fileStream.seekg(0, std::ios::end);
        deusize byteWidth = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        // Temporary memory
        DvigEngine2::MemoryObject* tempMemoryObject = DvigEngine2::Engine::Allocate( 0, byteWidth );
        geometryData = tempMemoryObject->Unwrap<deuchar*>();
        geometryDataByteWidth = byteWidth;

        // Read
        fileStream.read((char*)geometryData, geometryDataByteWidth);

        // Delete temporary memory
        engine->Delete( &tempMemoryObject );
    }

    // Copy to global geometry buffer
    DvigEngine2::GeometryBatch::m_GlobalGeometryBuffer->Insert( DV_NULL, geometryData, geometryDataByteWidth );
}

void DvigEngine2::GeometryComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}