#include <iostream>
#include "../include/DvigEngine.hpp"

void DvigEngine::ENTITY_DATA::Init(Engine* engine, Entity* entity)
{
    engine->GetData()->m_EntityCount += 1;
    
    m_SubStorageAddress = (void*)((dvmachword)entity + sizeof(Entity)); //Engine::Allocate(2, sizeof(IComponent));
    m_SubStorageByteWidth = 0; //sizeof(IComponent);
    m_ComponentCount = 0; //1;
    for (dvisize i = 0; i < DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT; ++i) {
        m_ComponentBits[i] = 0;
    }
}