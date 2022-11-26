#include "../../include/DECore.hpp"
#include "../../include/DEOpenGL4.hpp"
#include "../../include/DERendering.hpp"
#include <fstream>

void DvigEngine::GeometryComponent::Init(const char* optGeometryDataPathOnDrive, const char* optIndicesDataPathOnDrive, void* optGeometryData, void* optIndicesData, deusize optGeometryDataByteWidth, deusize optIndicesDataByteWidth)
{
    DvigEngine::Engine* engine = this->GetEngine();

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
        DvigEngine::MemoryObject* geometryTempMemoryObject = DvigEngine::Engine::Allocate( 0, geometryDataFileByteWidth );
        geometryData = geometryTempMemoryObject->Unwrap<deuchar*>();
        geometryDataByteWidth = geometryDataFileByteWidth;
        DvigEngine::MemoryObject* indicesTempMemoryObject = DvigEngine::Engine::Allocate( 0, indicesDataFileByteWidth );
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
    this->m_GeometryBufferOffset = DvigEngine::RenderingSystem::m_GlobalGeometryBuffer->Insert( DV_NULL, geometryData, geometryDataByteWidth );
    this->m_IndexBufferByteWidth = indicesDataByteWidth;
    this->m_IndexBufferOffset = DvigEngine::RenderingSystem::m_GlobalIndexBuffer->Insert( DV_NULL, indicesData, indicesDataByteWidth );

    // Update vertex buffer
    DvigEngine::GL4::BindBuffer( GL_ARRAY_BUFFER, DvigEngine::RenderingSystem::m_GLGlobalGeometryBuffer );
    DvigEngine::GL4::BufferSubData( GL_ARRAY_BUFFER, this->m_GeometryBufferOffset, geometryDataByteWidth, geometryData );
    DvigEngine::GL4::BindBuffer( GL_ARRAY_BUFFER, 0 );

    // Update index buffer
    DvigEngine::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, DvigEngine::RenderingSystem::m_GLGlobalIndexBuffer );
    DvigEngine::GL4::BufferSubData( GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBufferOffset, indicesDataByteWidth, indicesData );
    DvigEngine::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void DvigEngine::GeometryComponent::Init(const char* meshPathOnDrive)
{
    DvigEngine::Engine* engine = this->GetEngine();

    // Open file
    std::ifstream meshFileStream( &meshPathOnDrive[0], std::ios::binary );

    meshFileStream.seekg(0, std::ios::end);
    const deisize meshDataByteWidth = meshFileStream.tellg();
    meshFileStream.seekg(0, std::ios::beg);

    // Allocate temp memory object for mesh
    DvigEngine::MemoryObject* meshDataMemoryObject = DvigEngine::Engine::Allocate( 0, meshDataByteWidth );
    DvigEngine::deuchar* const meshData = meshDataMemoryObject->Unwrap<DvigEngine::deuchar* const>();

    // Allocate geometry buffer
    DvigEngine::deint32 meshVertexCount = 0;
    DvigEngine::MemoryObject* meshGeometryDataMemoryObject = DvigEngine::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );
    DvigEngine::GeometryVertex* meshGeometryData = meshGeometryDataMemoryObject->Unwrap<DvigEngine::GeometryVertex*>();

    // Allocate geometry position buffer
    DvigEngine::deint32 meshPositionCount = 0;
    DvigEngine::MemoryObject* meshGeometryPositionDataMemoryObject = DvigEngine::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );
    DvigEngine::defloat32* meshGeometryPositionData = meshGeometryPositionDataMemoryObject->Unwrap<DvigEngine::defloat32*>();

    // Allocate geometry texcoord buffer
    DvigEngine::deint32 meshTexcoordCount = 0;
    DvigEngine::MemoryObject* meshGeometryTexcoordDataMemoryObject = DvigEngine::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );
    DvigEngine::defloat32* meshGeometryTexcoordData = meshGeometryTexcoordDataMemoryObject->Unwrap<DvigEngine::defloat32*>();

    // Allocate geometry normal buffer
    DvigEngine::deint32 meshNormalCount = 0;
    DvigEngine::MemoryObject* meshGeometryNormalDataMemoryObject = DvigEngine::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );
    DvigEngine::defloat32* meshGeometryNormalData = meshGeometryNormalDataMemoryObject->Unwrap<DvigEngine::defloat32*>();

    // Allocate index buffer
    DvigEngine::deisize meshIndexCount = 0;
    DvigEngine::MemoryObject* meshIndexDataMemoryObject = DvigEngine::Engine::Allocate( 0, DV_MAX_GL_DEFAULT_BUFFER_BYTE_WIDTH );
    DvigEngine::deuint32* meshIndexData = meshIndexDataMemoryObject->Unwrap<DvigEngine::deuint32*>();

    // Read file
    meshFileStream.read((char*)meshData, meshDataByteWidth);

    // Close file
    meshFileStream.close();

    // Parse .obj data
    for (DvigEngine::deint32 i = 0; i < meshDataByteWidth;)
    {
        // Consume keyword
        DvigEngine::destring keyword = {};
        DvigEngine::deisize keywordByteWidth = 0;
        while (meshData[i] != ' ' && meshData[i] != '\t' && meshData[i] != '\n') {
            keyword[keywordByteWidth++] = meshData[i];
            i += 1;
        }

        // Process interesting keyword
        if (keyword[0] == 'v' && keywordByteWidth == 1)
        {
            // Vertex
            DvigEngine::destring nValue[3] = { {}, {}, {} };
            DvigEngine::deisize nValueCursor[3] = {};
            DvigEngine::deint32 curValueIndex = -1;
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
            meshGeometryPositionData[ meshPositionCount++ ] = xx;
            meshGeometryPositionData[ meshPositionCount++ ] = yy;
            meshGeometryPositionData[ meshPositionCount++ ] = zz;
        }
        else if (keyword[0] == 'v' && keyword[1] == 't' && keywordByteWidth == 2)
        {
            // Vertex
            DvigEngine::destring nValue[3] = { {}, {}, {} };
            DvigEngine::deisize nValueCursor[3] = {};
            DvigEngine::deint32 curValueIndex = -1;
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

            // Insert normal to buffer
            meshGeometryTexcoordData[ meshTexcoordCount++ ] = xx;
            meshGeometryTexcoordData[ meshTexcoordCount++ ] = yy;
        }
        else if (keyword[0] == 'v' && keyword[1] == 'n' && keywordByteWidth == 2)
        {
            // Vertex
            DvigEngine::destring nValue[3] = { {}, {}, {} };
            DvigEngine::deisize nValueCursor[3] = {};
            DvigEngine::deint32 curValueIndex = -1;
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

            // Insert normal to buffer
            meshGeometryNormalData[ meshNormalCount++ ] = xx;
            meshGeometryNormalData[ meshNormalCount++ ] = yy;
            meshGeometryNormalData[ meshNormalCount++ ] = zz;
        }
        else if (keyword[0] == 'f')
        {
            // Face
            DvigEngine::destring nValue[9] = { {}, {}, {} };
            DvigEngine::deisize nValueCursor[9] = {};
            DvigEngine::deint32 curValueIndex = -1;
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
            meshIndexData[ meshIndexCount++ ] = position1 > 0 ? position1 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = texcoord1 > 0 ? texcoord1 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = normal1 > 0 ? normal1 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = position2 > 0 ? position2 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = texcoord2 > 0 ? texcoord2 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = normal2 > 0 ? normal2 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = position3 > 0 ? position3 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = texcoord3 > 0 ? texcoord3 - 1 : 0;
            meshIndexData[ meshIndexCount++ ] = normal3 > 0 ? normal3 - 1 : 0;
        }
        else
        {
            // Skip until line ends
            while (meshData[i++] != '\n');
        }
    }

    // Setup geometry buffer
    meshVertexCount = 0;
    const deisize actualVertexCount = meshIndexCount / 3;
    for (deint32 i = 0; i < actualVertexCount; ++i)
    {
        const deint32 indexDataIndex = 3 * i;
        const deint32 indexPosition = meshIndexData[ indexDataIndex ];
        const deint32 indexTexcoord = meshIndexData[ 1 + indexDataIndex ];
        const deint32 indexNormal = meshIndexData[ 2 + indexDataIndex ];
        const deint32 positionDataIndex = 3 * indexPosition;
        const deint32 texcoordDataIndex = 2 * indexTexcoord;
        const deint32 normalDataIndex = 3 * indexNormal;
        meshGeometryData[ meshVertexCount ].m_Position[ 0 ] = meshGeometryPositionData[ positionDataIndex ];
        meshGeometryData[ meshVertexCount ].m_Position[ 1 ] = meshGeometryPositionData[ 1 + positionDataIndex ];
        meshGeometryData[ meshVertexCount ].m_Position[ 2 ] = meshGeometryPositionData[ 2 + positionDataIndex ];        
        meshGeometryData[ meshVertexCount ].m_Texcoord[ 0 ] = meshGeometryTexcoordData[ texcoordDataIndex ];
        meshGeometryData[ meshVertexCount ].m_Texcoord[ 1 ] = meshGeometryTexcoordData[ 1 + texcoordDataIndex ];
        meshGeometryData[ meshVertexCount ].m_Normal[ 0 ] = meshGeometryNormalData[ normalDataIndex ];
        meshGeometryData[ meshVertexCount ].m_Normal[ 1 ] = meshGeometryNormalData[ 1 + normalDataIndex ];
        meshGeometryData[ meshVertexCount ].m_Normal[ 2 ] = meshGeometryNormalData[ 2 + normalDataIndex ];        
        meshIndexData[ meshVertexCount ] = i;
        meshVertexCount += 1;
    }

    // Copy to global buffers
    this->m_IndexCount = meshVertexCount;
    const deusize meshGeometryDataByteWidth = sizeof(DvigEngine::GeometryVertex) * meshVertexCount;
    this->m_GeometryBufferByteWidth = meshGeometryDataByteWidth;
    this->m_GeometryBufferOffset = DvigEngine::RenderingSystem::m_GlobalGeometryBuffer->Insert( DV_NULL, meshGeometryData, meshGeometryDataByteWidth );
    const deusize meshIndexDataByteWidth = sizeof(DvigEngine::deuint32) * meshVertexCount;
    this->m_IndexBufferByteWidth = meshIndexDataByteWidth;
    this->m_IndexBufferOffset = DvigEngine::RenderingSystem::m_GlobalIndexBuffer->Insert( DV_NULL, meshIndexData, meshIndexDataByteWidth );

    // Update vertex buffer
    DvigEngine::GL4::BindBuffer( GL_ARRAY_BUFFER, DvigEngine::RenderingSystem::m_GLGlobalGeometryBuffer );
    DvigEngine::GL4::BufferSubData( GL_ARRAY_BUFFER, this->m_GeometryBufferOffset, meshGeometryDataByteWidth, meshGeometryData );
    DvigEngine::GL4::BindBuffer( GL_ARRAY_BUFFER, 0 );

    // Update index buffer
    DvigEngine::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, DvigEngine::RenderingSystem::m_GLGlobalIndexBuffer );
    DvigEngine::GL4::BufferSubData( GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBufferOffset, meshIndexDataByteWidth, meshIndexData );
    DvigEngine::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    // Delete temp memory objects
    engine->Delete( meshDataMemoryObject );
    engine->Delete( meshGeometryDataMemoryObject );
    engine->Delete( meshGeometryPositionDataMemoryObject );
    engine->Delete( meshGeometryTexcoordDataMemoryObject );
    engine->Delete( meshGeometryNormalDataMemoryObject );
    engine->Delete( meshIndexDataMemoryObject );
}

void DvigEngine::GeometryComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}