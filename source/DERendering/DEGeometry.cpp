#include "../../include/dvigengine/DECore.hpp"
#include "../../include/dvigengine/DED3D11.hpp"
#include "../../include/dvigengine/DERendering.hpp"
#include <fstream>
#include <iostream>

void DvigEngine::GeometryComponent::Init(const char* meshPathOnDrive)
{
    Engine* engine = Engine::GetClassInstance();

    // Open file
    std::ifstream meshFileStream( &meshPathOnDrive[0], std::ios::binary );

    meshFileStream.seekg(0, std::ios::end);
    const ssize meshDataByteWidth = (const ssize)meshFileStream.tellg();
    meshFileStream.seekg(0, std::ios::beg);

    // Allocate temp memory object for mesh
    MemoryObject* meshDataMemoryObject = engine->MemoryAllocate( 0, meshDataByteWidth );
    uint8* const meshData = (uint8* const)meshDataMemoryObject->GetAddress();

    // Allocate geometry buffer
    sint32 meshVertexCount = 0;
    MemoryObject* meshGeometryDataMemoryObject = engine->MemoryAllocate( 0, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
    GeometryVertex* meshGeometryData = (GeometryVertex*)meshGeometryDataMemoryObject->GetAddress();

    // Allocate geometry position buffer
    sint32 meshPositionCount = 0;
    MemoryObject* meshGeometryPositionDataMemoryObject = engine->MemoryAllocate( 0, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
    mfloat* meshGeometryPositionData = (mfloat*)meshGeometryPositionDataMemoryObject->GetAddress();

    // Allocate geometry texcoord buffer
    sint32 meshTexcoordCount = 0;
    MemoryObject* meshGeometryTexcoordDataMemoryObject = engine->MemoryAllocate( 0, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
    mfloat* meshGeometryTexcoordData = (mfloat*)meshGeometryTexcoordDataMemoryObject->GetAddress();

    // Allocate geometry normal buffer
    sint32 meshNormalCount = 0;
    MemoryObject* meshGeometryNormalDataMemoryObject = engine->MemoryAllocate( 0, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
    mfloat* meshGeometryNormalData = (mfloat*)meshGeometryNormalDataMemoryObject->GetAddress();

    // Allocate index buffer
    ssize meshIndexCount = 0;
    MemoryObject* meshIndexDataMemoryObject = engine->MemoryAllocate( 0, DVIG_MAX_GFX_BUFFER_BYTE_WIDTH );
    uint32* meshIndexData = (uint32*)meshIndexDataMemoryObject->GetAddress();

    // Read file
    meshFileStream.read((char*)meshData, meshDataByteWidth);

    // Close file
    meshFileStream.close();

    // Parse .obj data
    for (sint32 i = 0; i < meshDataByteWidth;)
    {
        // Consume keyword
        uint8 keyword[DVIG_MAX_STRING_BYTE_WIDTH] = {};
        ssize keywordByteWidth = 0;
        while (meshData[i] != ' ' && meshData[i] != '\t' && meshData[i] != '\n') {
            keyword[keywordByteWidth++] = meshData[i];
            i += 1;
        }

        // Process interesting keyword
        if (keyword[0] == 'v' && keywordByteWidth == 1)
        {
            // Vertex position
            uint8 nValue[3][DVIG_MAX_STRING_BYTE_WIDTH] = { {}, {}, {} };
            ssize nValueCursor[3] = {};
            sint32 curValueIndex = -1;
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
            // Vertex texcoord
            uint8 nValue[3][DVIG_MAX_STRING_BYTE_WIDTH] = { {}, {}, {} };
            ssize nValueCursor[3] = {};
            sint32 curValueIndex = -1;
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
            // Vertex normal
            uint8 nValue[3][DVIG_MAX_STRING_BYTE_WIDTH] = { {}, {}, {} };
            ssize nValueCursor[3] = {};
            sint32 curValueIndex = -1;
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
            uint8 nValue[9][DVIG_MAX_STRING_BYTE_WIDTH] = { {}, {}, {} };
            ssize nValueCursor[9] = {};
            sint32 curValueIndex = -1;
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
    const ssize actualVertexCount = meshIndexCount / 3;
    for (sint32 i = 0; i < actualVertexCount; ++i)
    {
        const sint32 indexDataIndex = 3 * i;
        const sint32 indexPosition = meshIndexData[ indexDataIndex ];
        const sint32 indexTexcoord = meshIndexData[ 1 + indexDataIndex ];
        const sint32 indexNormal = meshIndexData[ 2 + indexDataIndex ];
        const sint32 positionDataIndex = 3 * indexPosition;
        const sint32 texcoordDataIndex = 2 * indexTexcoord;
        const sint32 normalDataIndex = 3 * indexNormal;
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
    const usize meshGeometryDataByteWidth = sizeof(GeometryVertex) * meshVertexCount;
    this->m_GeometryBufferByteWidth = meshGeometryDataByteWidth;
    this->m_GeometryBufferOffset = RenderingSystem::m_GlobalVertexBufferOffset;
    const usize meshIndexDataByteWidth = sizeof(uint32) * meshVertexCount;
    this->m_IndexBufferByteWidth = meshIndexDataByteWidth;
    this->m_IndexBufferOffset = RenderingSystem::m_GlobalIndexBufferOffset;

    // Increase global buffer offsets
    RenderingSystem::m_GlobalVertexBufferOffset += this->m_GeometryBufferByteWidth;
    RenderingSystem::m_GlobalIndexBufferOffset += this->m_IndexBufferByteWidth;

    // Update D3D buffers
    DvigEngine::D3D11::WriteToBuffer( RenderingSystem::m_GlobalVertexBuffer, meshGeometryData, this->m_GeometryBufferOffset, this->m_GeometryBufferByteWidth );
    DvigEngine::D3D11::WriteToBuffer( RenderingSystem::m_GlobalIndexBuffer, meshIndexData, this->m_IndexBufferOffset, this->m_IndexBufferByteWidth );

    // Delete temp memory objects
    engine->MemoryDelete( meshDataMemoryObject );
    engine->MemoryDelete( meshGeometryDataMemoryObject );
    engine->MemoryDelete( meshGeometryPositionDataMemoryObject );
    engine->MemoryDelete( meshGeometryTexcoordDataMemoryObject );
    engine->MemoryDelete( meshGeometryNormalDataMemoryObject );
    engine->MemoryDelete( meshIndexDataMemoryObject );
}

void DvigEngine::GeometryComponent::Free()
{
    Engine::GetClassInstance()->MemoryDelete( this->GetMemoryObject() );
}