#include <iostream>
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

    String* myString;
    engine->Create<String>((const void** const)&myString, "MyStringUniqueID", nullptr);
    
    std::cout << "Success!" << std::endl;
    
    Engine::Free();

    return 0;
}