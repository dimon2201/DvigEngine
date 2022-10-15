#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <thread>
#include "../include/DvigEngine.hpp"

using namespace DvigEngine;

class MemoryPoolShell : IShell
{
    public:
        void printMemoryPoolInfo(Engine* engine)
        {
            MEMORY_POOL_DATA* mp0 = (MEMORY_POOL_DATA*)engine->GetMemoryPoolByID(0)->GetData();
            MEMORY_POOL_DATA* mp1 = (MEMORY_POOL_DATA*)engine->GetMemoryPoolByID(1)->GetData();
            
            std::cout << "Pool 0 Address : " << (dvmachword)mp0->m_Address << std::endl;
            std::cout << "Pool 1 Address : " << (dvmachword)mp1->m_Address << std::endl;
        }
};

int main()
{
    MEMORY_POOL_DATA memoryPoolsData[3];
    memoryPoolsData[0].m_ByteWidth = 65536;
    memoryPoolsData[1].m_ByteWidth = 65536;
    memoryPoolsData[2].m_ByteWidth = 65536;

    ENGINE_INPUT_DATA engineInputData;
    engineInputData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineInputData.m_MemoryPoolsCount = 2u;
    engineInputData.m_MemoryPoolsData = memoryPoolsData;
    engineInputData.m_ReservedMemoryPoolID = 1;
    engineInputData.m_StorageMemoryPoolID = 2;
    engineInputData.m_RequestedThreadCount = 1;

    Engine::Init(&engineInputData);
    Engine* engine = Engine::GetInstance();
    
    clock_t ts = clock();
    engine->StartThreads();

    // Create strings
    String* strings[4]; // memory for 2 Strings
    engine->Create<String>((const void** const)&strings[0], "MyStringID_0", nullptr);
    engine->Create<String>((const void** const)&strings[1], "MyStringID_1", nullptr);
    engine->Create<String>((const void** const)&strings[2], "MyStringID_2", nullptr);
    engine->Create<String>((const void** const)&strings[3], "MyStringID_3", nullptr);
    strings[0][0] = "Hello";
    strings[1][0] = " world!";
    strings[2][0] = " how are you";
    strings[3][0] = " doing?";
    
    // DV_XMACRO_CREATE_STRING_GLOBAL_SCOPE_CAPACITY(4);
    // DV_XMACRO_CREATE_STRING(myString, "MyStringID_Second", "How are you doing?");
    // DV_XMACRO_CREATE_STRING(anotherString, "MyStringID_Second2", "...");
    // DV_XMACRO_CREATE_STRING(andAnotherString, "MyStringID_Second22", "Wow!");
    // engine->DeleteChunk( myString->m_MemoryObject );
    // std::cout << (dvmachword)strings[0] << std::endl;
    std::cout << (dvmachword)strings[1] << std::endl;

    engine->DeleteChunk( strings[1]->m_MemoryObject );

    // void* address = strings[0]->m_MemoryObject;
    // dvmachword toDeleteByteWidth = sizeof(DvigEngine::MemoryChunk) + ((MemoryChunk*)address)->GetData()->m_ByteWidth;
    // void* nextAddress = (void*)((dvmachword)address + toDeleteByteWidth);
    // IObject* obj = (IObject*)(((MemoryChunk*)address)->GetData()->m_Address);
    // IObject** ppObj = (IObject**)obj->m_Createe;
    // IObject* pNObj = (IObject*)(((MemoryChunk*)nextAddress)->GetData()->m_Address);
    // IObject** ppNObj = &pNObj;
    // *ppObj = pNObj;
    // ppObj = ppNObj;
    // obj->m_Createe = (IObject**)ppNObj;
    // *obj->m_Createe = (IObject*)pNObj;
    // std::cout << "ppObj : " << (dvmachword)*obj->m_Createe << " / " << (dvmachword)strings[0] << std::endl;
    
    // Create entities
    // Entity* entities[2];
    // engine->Create<Entity>((const void** const)&entities[0], "EntityID_0", nullptr);

    // HashMap* hashMaps[2];
    // engine->Create<String>((const void** const)&hashMaps[0], "MyHashMap", nullptr);
    // hashMaps[0]->Insert(  );

    engine->StopThreads();
    clock_t te = clock();
    
    std::cout << "Output : " << strings[0]->GetData()->m_Chars
                             << strings[2]->GetData()->m_Chars << strings[3]->GetData()->m_Chars
                             << std::endl;
    std::cout << "Success!" << std::endl;
    std::cout << "Time : " << te - ts << std::endl;

    Engine::Free();

    return 0;
}