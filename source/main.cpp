#include <iostream>
#include "../include/DvigEngine.h"

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
    MEMORY_POOL_DATA memoryPoolsData[2];
    memoryPoolsData[0].m_ByteWidth = 65536;
    memoryPoolsData[1].m_ByteWidth = 65536;

    ENGINE_USER_DATA engineUserData;
    engineUserData.m_Version = DV_ENGINE_VERSION_NUMBER;
    engineUserData.m_MemoryPoolsCount = 2u;
    engineUserData.m_MemoryPoolsData = memoryPoolsData;
    engineUserData.m_ReservedMemoryPoolID = 1;

    Engine::Init(&engineUserData);
    Engine* engine = Engine::GetInstance();
    
    STRING_DATA stringData[4];
    stringData[0] = STRING_DATA("id0");
    stringData[1] = STRING_DATA("brick");
    stringData[2] = STRING_DATA("prop_box01");
    stringData[3] = STRING_DATA("prop_papers01");
    void* mem = Engine::Allocate(0, sizeof(String));
    String* s = engine->Create<String>("id0", &stringData[0]);
    memcpy(mem, s, sizeof(String));

    HASH_MAP_DATA hashMapData = HASH_MAP_DATA();
    hashMapData.m_AssocAddress = mem;
    hashMapData.m_AssocEntrySize = sizeof(String);
    auto hashMap = engine->Create<HashMap>("hashmap0", &hashMapData);
    hashMap->Insert(s, mem);

    void* key0Val = hashMap->Find(s);
    std::cout << (dvmachword)mem << std::endl;
    std::cout << (dvmachword)key0Val << std::endl;
    
    // std::cout << "Core Address             : " << (dvmachword)engine->GetData() << std::endl;

    // Engine::Free();

    return 0;
}