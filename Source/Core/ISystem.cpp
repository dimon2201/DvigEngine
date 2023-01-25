#include "../../Include/DvigEngine/Core.hpp"

void DvigEngine::ISystem::Update()
{
    Engine* engine = Engine::GetClassInstance();

    MemoryPoolInfo* memoryPoolInfo = engine->GetMemoryPoolInfo(1);
    void* componentStorageAddress = memoryPoolInfo->m_Address;
    void* componentStorageLastAddress = Ptr<void*>::Add( componentStorageAddress, memoryPoolInfo->m_OccupiedByteWidth );
    componentStorageLastAddress = Ptr<void*>::Subtract( componentStorageLastAddress, sizeof(MemoryObject) );

    while (componentStorageAddress < componentStorageLastAddress)
    {
        MemoryObject* memoryObject = (MemoryObject*)componentStorageAddress;
        Component* component = (Component*)memoryObject->GetAddress();
        this->UpdateComponent(component);

        componentStorageAddress = Ptr<void*>::Add( componentStorageAddress, sizeof(MemoryObject) + memoryObject->GetSize());
    }
}