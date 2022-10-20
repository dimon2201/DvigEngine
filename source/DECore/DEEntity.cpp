#include "../../include/DECore.hpp"

DvigEngine::deuint32 DvigEngine::Entity::m_EntityCount = 0u;

void DvigEngine::ENTITY_DATA::Init(Engine* engine, Entity* entity)
{
    Entity::m_EntityCount += 1;
    
    m_SubStorageAddress = (void*)((demachword)entity + sizeof(Entity)); //Engine::Allocate(2, sizeof(IComponent));
    m_SubStorageByteWidth = 0; //sizeof(IComponent);
    m_ComponentCount = 0; //1;
    for (deisize i = 0; i < DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT; ++i) {
        m_ComponentBits[i] = 0;
    }
}