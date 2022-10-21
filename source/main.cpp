#include "../include/DECore.hpp"

using namespace DvigEngine;

int main()
{
    MEMORY_POOL_DATA memoryPoolsData[3];
    memoryPoolsData[0].m_ByteWidth = 465536;
    memoryPoolsData[1].m_ByteWidth = 465536;
    memoryPoolsData[2].m_ByteWidth = 465536;

    ENGINE_INPUT_DATA engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 3u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_SystemMemoryPoolID = 1;
    engineInputData.m_StorageMemoryPoolID = 2;
    engineInputData.m_RequestedThreadCount = 1;

    Engine::Init(&engineInputData);
    Engine* engine = Engine::GetInstance();

    DynamicBuffer* dyBuffers[1];
    engine->Create<DynamicBuffer>((const void**)&dyBuffers[0], "MyDynamicBuffer_0", nullptr);

    deint32 value = 255;
    dyBuffers[0]->CopyToBuffer( &value, sizeof(deint32) );
    dyBuffers[0]->RequestFromBuffer( 0, &value, sizeof(deint32) );
    std::cout << value << std::endl;
    
    Engine::Free();

    // Prototype** monster;
    // engine->Create <Prototype> ( monster );
    // monster->AddComponent <Geometry> ( &mesh );
    // monster->AddComponent <Script> ( &script );

    // Instance* monster_Instance0 = monster->Instantiate(  );
    // monster_Instance0->SetComponent <Script> ( &userScript );

    std::cout << "Success!" << std::endl;

    return 0;
}