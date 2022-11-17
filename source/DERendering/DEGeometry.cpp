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

        // Close files
        geometryFileStream.close();
        indicesFileStream.close();

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

void DvigEngine2::GeometryComponent::Init(const char* meshPathOnDrive)
{
    DvigEngine2::Engine* engine = this->GetEngine();

    // Open file
    std::ifstream meshFileStream( &meshPathOnDrive[0], std::ios::binary );

    meshFileStream.seekg(0, std::ios::end);
    const deusize meshDataByteWidth = meshFileStream.tellg();
    meshFileStream.seekg(0, std::ios::beg);

    // Allocate temp memory object for mesh
    // DvigEngine2::MemoryObject* meshDataMemoryObject = DvigEngine2::Engine::Allocate( 0, meshDataByteWidth );
    DvigEngine2::deuchar* const meshData = (DvigEngine2::deuchar* const)malloc(DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH);//meshDataMemoryObject->Unwrap<DvigEngine2::deuchar* const>();

    // Allocate geometry buffer
    DvigEngine2::deint32 meshPointCount = 0;
    // DvigEngine2::MemoryObject* meshGeometryDataMemoryObject = DvigEngine2::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );
    DvigEngine2::defloat32* meshGeometryData = (DvigEngine2::defloat32*)malloc(DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH);//meshGeometryDataMemoryObject->Unwrap<DvigEngine2::defloat32*>();

    // Allocate index buffer
    DvigEngine2::deisize meshIndexCount = 0;
    // DvigEngine2::MemoryObject* meshIndexDataMemoryObject = DvigEngine2::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );
    DvigEngine2::deuint32* meshIndexData = (DvigEngine2::deuint32*)malloc(DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH);//meshIndexDataMemoryObject->Unwrap<DvigEngine2::deuint32*>();

    // Read file
    meshFileStream.read((char*)meshData, meshDataByteWidth);

    // Close file
    meshFileStream.close();

    // Parse .obj data
    for (DvigEngine2::deint32 i = 0; i < meshDataByteWidth;)
    {
        // Consume keyword
        DvigEngine2::destring keyword = {};
        DvigEngine2::deisize keywordByteWidth = 0;
        while (meshData[i] != ' ' && meshData[i] != '\t' && meshData[i] != '\n') {
            keyword[keywordByteWidth++] = meshData[i];
            i += 1;
        }

        // Process interesting keyword
        if (keyword[0] == 'v' && keywordByteWidth == 1)
        {
            // Vertex
            DvigEngine2::destring nValue[3] = { {}, {}, {} };
            DvigEngine2::deisize nValueCursor[3] = {};
            DvigEngine2::deint32 curValueIndex = -1;
            while (i < meshDataByteWidth && meshData[i] != '\n')
            {
                while (meshData[i] == ' ' || meshData[i] == '\t') { i += 1; }
                if (meshData[i - 1] == ' ' || meshData[i - 1] == '\t') { curValueIndex += 1; }
                nValue[curValueIndex][nValueCursor[curValueIndex]++] = meshData[i];
                i += 1;
            }
            
            // Convert strings to floats
            char* end;
            float xx = std::strtof( (const char*)&nValue[0], &end );
            float yy = std::strtof( (const char*)&nValue[1], &end );
            float zz = std::strtof( (const char*)&nValue[2], &end );

            // // Insert vertex to buffer
            meshGeometryData[ meshPointCount++ ] = xx;
            meshGeometryData[ meshPointCount++ ] = yy;
            meshGeometryData[ meshPointCount++ ] = zz;
        }
        else if (keyword[0] == 'f')
        {
            // Face
            DvigEngine2::destring nValue[9] = { {}, {}, {} };
            DvigEngine2::deisize nValueCursor[9] = {};
            DvigEngine2::deint32 curValueIndex = -1;
            while (i < meshDataByteWidth && meshData[i] != '\n')
            {
                while (meshData[i] == '/' || meshData[i] == ' ' || meshData[i] == '\t') {
                    if (meshData[i - 1] == '/') { curValueIndex += 1; }
                    i += 1;
                }
                if (meshData[i - 1] == '/' || meshData[i - 1] == ' ' || meshData[i - 1] == '\t') { curValueIndex += 1; }
                nValue[curValueIndex][nValueCursor[curValueIndex]++] = meshData[i];
                i += 1;
            }

            char* end;
            long position1 = std::strtoul( (const char*)&nValue[0], &end, 10 );
            long texcoord1 = std::strtoul( (const char*)&nValue[1], &end, 10 );
            long normal1 = std::strtoul( (const char*)&nValue[2], &end, 10 );
            long position2 = std::strtoul( (const char*)&nValue[3], &end, 10 );
            long texcoord2 = std::strtoul( (const char*)&nValue[4], &end, 10 );
            long normal2 = std::strtoul( (const char*)&nValue[5], &end, 10 );
            long position3 = std::strtoul( (const char*)&nValue[6], &end, 10 );
            long texcoord3 = std::strtoul( (const char*)&nValue[7], &end, 10 );
            long normal3 = std::strtoul( (const char*)&nValue[8], &end, 10 );

            // Insert vertex to buffer
            meshIndexData[ meshIndexCount++ ] = position1 - 1;
            meshIndexData[ meshIndexCount++ ] = position2 - 1;
            meshIndexData[ meshIndexCount++ ] = position3 - 1;
            while (meshData[i++] != '\n');
        }
        else
        {
            // Skip until line ends
            while (meshData[i++] != '\n');
        }
    }

    // Copy to global buffers
    const deusize meshGeometryDataByteWidth = sizeof(DvigEngine2::defloat32) * meshPointCount;
    this->m_GeometryBufferByteWidth = meshGeometryDataByteWidth;
    std::cout << "geometry : " << meshGeometryDataByteWidth << std::endl;
    this->m_GeometryBufferOffset = DvigEngine2::RenderingSystem::m_GlobalGeometryBuffer->Insert( DV_NULL, meshGeometryData, meshGeometryDataByteWidth );
    // const deusize meshIndexDataByteWidth = sizeof(DvigEngine2::deuint32) * meshIndexCount;
    // this->m_IndexBufferByteWidth = meshIndexDataByteWidth;
    // this->m_IndexBufferOffset = DvigEngine2::RenderingSystem::m_GlobalIndexBuffer->Insert( DV_NULL, meshIndexData, meshIndexDataByteWidth );

    // for (int i = 0; i < 9; ++i) {
    //     std::cout << meshGeometryData[i] << " ";
    // }
    // std::cout << std::endl;
    // // for (int i = 0; i < 6; ++i) {
    // //     std::cout << meshIndexData[i] << " ";
    // // }
    // // std::cout << std::endl;

    // Update vertex buffer
    // DvigEngine2::GL4::BindBuffer( GL_ARRAY_BUFFER, DvigEngine2::RenderingSystem::m_GLGlobalGeometryBuffer );
    // DvigEngine2::GL4::BufferSubData( GL_ARRAY_BUFFER, this->m_GeometryBufferOffset, meshGeometryDataByteWidth, meshGeometryData );
    // DvigEngine2::GL4::BindBuffer( GL_ARRAY_BUFFER, 0 );

    // // // Update index buffer
    // DvigEngine2::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, DvigEngine2::RenderingSystem::m_GLGlobalIndexBuffer );
    // DvigEngine2::GL4::BufferSubData( GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBufferOffset, meshIndexDataByteWidth, meshIndexData );
    // DvigEngine2::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    // Delete temp memory object
    // engine->Delete( meshDataMemoryObject );
    // engine->Delete( meshGeometryDataMemoryObject );
    // engine->Delete( meshIndexDataMemoryObject );
}

void DvigEngine2::GeometryComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}