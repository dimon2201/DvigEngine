#include "../../include/DECore.hpp"
#include "../../include/DEGraphics.hpp"

#include <fstream>

void DvigEngine2::GeometryComponent::Initx(const char* optGeometryPathOnDrive, void* optGeometryData, deusize optGeometryDataByteWidth)
{
    // Init internal data
    DvigEngine2::Engine* engine = this->GetEngine();
    engine->Create<DvigEngine2::DynamicBuffer>( &this->m_VertexBuffer, "_VertexBuffer", nullptr );
    this->m_VertexBuffer->Init( 0, 1024 );

    // Geometry data address
    DvigEngine2::MemoryObject* tempMemoryObject;
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
        tempMemoryObject = DvigEngine2::Engine::Allocate( 0, byteWidth );
        geometryData = tempMemoryObject->Unwrap<deuchar*>();
        geometryDataByteWidth = byteWidth;

        // Read
        fileStream.read((char*)geometryData, geometryDataByteWidth);
    }

    // Copy to vertex buffer
    this->m_VertexBuffer->Insert( DV_NULL, geometryData, geometryDataByteWidth );
}