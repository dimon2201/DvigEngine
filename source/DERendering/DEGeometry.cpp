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
    DvigEngine2::MemoryObject* meshDataMemoryObject = DvigEngine2::Engine::Allocate( 0, meshDataByteWidth );
    DvigEngine2::deuchar* const meshData = meshDataMemoryObject->Unwrap<DvigEngine2::deuchar* const>();

    // Allocate geometry buffer
    DvigEngine2::deint32 meshPointCount = 0;
    DvigEngine2::MemoryObject* meshGeometryDataMemoryObject = DvigEngine2::Engine::Allocate( 0, 1024 );
    DvigEngine2::defloat32* meshGeometryData = meshGeometryDataMemoryObject->Unwrap<DvigEngine2::defloat32*>();

    // Allocate index buffer
    DvigEngine2::deisize meshIndexCount = 0;
    DvigEngine2::MemoryObject* meshIndexDataMemoryObject = DvigEngine2::Engine::Allocate( 0, 1024 );
    DvigEngine2::deuint32* meshIndexData = meshIndexDataMemoryObject->Unwrap<DvigEngine2::deuint32*>();

    // Read file
    meshFileStream.read((char*)meshData, meshDataByteWidth);

    // Close file
    meshFileStream.close();

    // Parse .obj data
    DvigEngine2::debool skipLine = DV_FALSE;
    for (DvigEngine2::deint32 i = 0; i < meshDataByteWidth; ++i)
    {
        // Consume keyword
        DvigEngine2::destring keyword = {};
        
        if (meshData[i] == '#')
		{
			skipLine = DV_TRUE;
			i += 1;
			keyword[0] = '#';
		}
		else if (   meshData[i] == 'm' && meshData[1 + i] == 't' && meshData[2 + i] == 'l' &&
                    meshData[3 + i] == 'l' && meshData[4 + i] == 'i' && meshData[5 + i] == 'b')
		{
			skipLine = DV_TRUE;
			i += 6;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "mtllib", 6 );
		}
		else if (   meshData[i] == 'u' && meshData[1 + i] == 's' && meshData[2 + i] == 'e' &&
                    meshData[3 + i] == 'm' && meshData[4 + i] == 't' && meshData[5 + i] == 'l')
		{
			skipLine = DV_TRUE;
			i += 6;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "usemtl", 6 );
		}
		else if (meshData[i] == 'g')
		{
			skipLine = DV_TRUE;
			i += 1;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "g", 1 );
		}
		else if (meshData[i] == 'o')
		{
			skipLine = DV_TRUE;
			i += 1;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "o", 1 );
		}
		else if (meshData[i] == 's')
		{
			skipLine = DV_TRUE;
			i += 1;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "s", 1 );
		}
		else if (meshData[i] == 'v' && meshData[1 + i] == ' ')
		{
			skipLine = DV_FALSE;
			i += 2;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "v ", 2 );
		}
		else if (meshData[i] == 'v' && meshData[1 + i] == 't')
		{
			skipLine = DV_FALSE;
			i += 2;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "vt", 2 );
		}
		else if (meshData[i] == 'v' && meshData[1 + i] == 'n')
		{
			skipLine = DV_FALSE;
			i += 2;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "vn", 2 );
		}
		else if (meshData[i] == 'f')
		{
			skipLine = DV_FALSE;
			i += 1;
			DvigEngine2::Engine::MemoryCopy( &keyword[0], "f", 1 );
		}
		else
		{
			skipLine = DV_TRUE;
			i += 1;
            keyword[0] = 0;
		}

        // Skip line only if needed
        if (skipLine == DV_TRUE) {
            while (meshData[i] != '\n' && i++ < meshDataByteWidth);
            skipLine = DV_FALSE;
            continue;
        }

        // Process interesting keyword
        if (keyword[0] == 'v' && keyword[1] == ' ')
        {
            // 'v ' = vertex
            // Consume X
            DvigEngine2::destring nX = {};
            DvigEngine2::deisize nXCount = 0;
            while (meshData[i] == ' ' || meshData[i] == '\t' || meshData[i] == '/' || meshData[i] == '\n') {
			    if (i >= meshDataByteWidth) { break; } i += 1;
		    }
            while (i < meshDataByteWidth && meshData[i] != ' ' && meshData[i] != '\t' && meshData[i] != '/' && meshData[i] != '\n') {
			    nX[nXCount++] = meshData[i++];
		    }

            // Consume Y
            DvigEngine2::destring nY = {};
            DvigEngine2::deisize nYCount = 0;
            while (meshData[i] == ' ' || meshData[i] == '\t' || meshData[i] == '/' || meshData[i] == '\n') {
			    if (i >= meshDataByteWidth) { break; } i += 1;
		    }
            while (i < meshDataByteWidth && meshData[i] != ' ' && meshData[i] != '\t' && meshData[i] != '/' && meshData[i] != '\n') {
			    nY[nYCount++] = meshData[i++];
		    }

            // Consume Z
            DvigEngine2::destring nZ = {};
            DvigEngine2::deisize nZCount = 0;
            while (meshData[i] == ' ' || meshData[i] == '\t' || meshData[i] == '/' || meshData[i] == '\n') {
			    if (i >= meshDataByteWidth) { break; } i += 1;
		    }
            while (i < meshDataByteWidth && meshData[i] != ' ' && meshData[i] != '\t' && meshData[i] != '/' && meshData[i] != '\n') {
			    nZ[nZCount++] = meshData[i++];
		    }
            
            // Convert strings to floats
            char* end;
            float xx = std::strtof( (const char*)&nX[0], &end );
            float yy = std::strtof( (const char*)&nY[0], &end );
            float zz = std::strtof( (const char*)&nZ[0], &end );

            // Insert vertex to buffer
            meshGeometryData[ meshPointCount++ ] = xx;
            meshGeometryData[ meshPointCount++ ] = yy;
            meshGeometryData[ meshPointCount++ ] = zz;
        }
        else
        // 'f' - Face
		if (keyword[0] == 'f')
        {
            DvigEngine2::destring nArray[9] = { {}, {}, {} };
            DvigEngine2::deisize nArrayCount[9] = {};
            DvigEngine2::deint32 arrayCursor = 0;
            DvigEngine2::debool meetSeparator = DV_FALSE;
            for (;;)
            {
                if (i >= meshDataByteWidth) { break; }
                while (i < meshDataByteWidth && (meshData[i] == ' ' || meshData[i] == '/')) {
                    meetSeparator = DV_TRUE;
                    if (meshData[i] == '/') {
                        arrayCursor += 1;
                    }
                    i += 1;
                }
                if (meshData[i] == '\n') { break; }

                if (meshData[i - 1] == ' ' && arrayCursor > 0) {
                    arrayCursor += 1;
                }

                nArray[arrayCursor][nArrayCount[arrayCursor]] = meshData[i];
                nArrayCount[arrayCursor] += 1;
                i += 1;
            }
            
            // Convert strings to floats
            char* end;
            float position1 = std::strtof( (const char*)&nArray[0], &end );
            float texcoord1 = std::strtof( (const char*)&nArray[1], &end );
            float normal1 = std::strtof( (const char*)&nArray[2], &end );
            float position2 = std::strtof( (const char*)&nArray[3], &end );
            float texcoord2 = std::strtof( (const char*)&nArray[4], &end );
            float normal2 = std::strtof( (const char*)&nArray[5], &end );
            float position3 = std::strtof( (const char*)&nArray[6], &end );
            float texcoord3 = std::strtof( (const char*)&nArray[7], &end );
            float normal3 = std::strtof( (const char*)&nArray[8], &end );
            meshIndexData[ meshIndexCount++ ] = position1 - 1u;
            meshIndexData[ meshIndexCount++ ] = position2 - 1u;
            meshIndexData[ meshIndexCount++ ] = position3 - 1u;
        }
    }

    // Copy to global buffers
    const deusize meshGeometryDataByteWidth = sizeof(DvigEngine2::defloat32) * meshPointCount;
    this->m_GeometryBufferByteWidth = meshGeometryDataByteWidth;
    this->m_GeometryBufferOffset = DvigEngine2::RenderingSystem::m_GlobalGeometryBuffer->Insert( DV_NULL, meshGeometryData, meshGeometryDataByteWidth );
    const deusize meshIndexDataByteWidth = sizeof(DvigEngine2::deuint32) * meshIndexCount;
    this->m_IndexBufferByteWidth = meshIndexDataByteWidth;
    this->m_IndexBufferOffset = DvigEngine2::RenderingSystem::m_GlobalIndexBuffer->Insert( DV_NULL, meshIndexData, meshIndexDataByteWidth );

    std::cout << std::endl;
    for (int i = 0; i < 9; ++i) {
        std::cout << meshGeometryData[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < 6; ++i) {
        std::cout << meshIndexData[i] << " ";
    }
    std::cout << std::endl;

    // Update vertex buffer
    DvigEngine2::GL4::BindBuffer( GL_ARRAY_BUFFER, DvigEngine2::RenderingSystem::m_GLGlobalGeometryBuffer );
    DvigEngine2::GL4::BufferSubData( GL_ARRAY_BUFFER, this->m_GeometryBufferOffset, meshGeometryDataByteWidth, meshGeometryData );
    DvigEngine2::GL4::BindBuffer( GL_ARRAY_BUFFER, 0 );

    // Update index buffer
    DvigEngine2::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, DvigEngine2::RenderingSystem::m_GLGlobalIndexBuffer );
    DvigEngine2::GL4::BufferSubData( GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBufferOffset, meshIndexDataByteWidth, meshIndexData );
    DvigEngine2::GL4::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    // Delete temp memory object
    engine->Delete( meshDataMemoryObject );
    engine->Delete( meshGeometryDataMemoryObject );
    engine->Delete( meshIndexDataMemoryObject );
}

void DvigEngine2::GeometryComponent::Free()
{
    this->GetEngine()->Delete( this->GetMemoryObject() );
}