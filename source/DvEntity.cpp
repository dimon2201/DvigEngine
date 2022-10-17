#include "../include/DvigEngine.hpp"

DvigEngine::ENTITY_DATA::ENTITY_DATA()
{
    m_SubStorageAddress = Engine::Allocate(2, sizeof(IComponent));
    m_SubStorageByteWidth = sizeof(IComponent);
    m_ComponentCount = 1;
    for (dvisize i = 0; i < DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT; ++i) {
        m_ComponentBits[i] = 0;
    }

    IComponent* component = (IComponent*)m_SubStorageAddress;
    dvuchar* typeName = (dvuchar*)typeid(IComponent).name();
    const dvusize typeNameByteWidth = String::CharactersCount( typeName );
    Engine::CopyMemory( &component->m_TypeName[0], &typeName[0], typeNameByteWidth );
    component->m_TypeName[typeNameByteWidth] = 0;
    component->m_LayoutByteWidth = sizeof(IComponent);
    component->m_RegistryIndex = DV_NULL;
}