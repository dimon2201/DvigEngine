#include "../../include/DECore.hpp"

void DvigEngine::PROTOTYPE_DATA::Init(Engine* engine, Prototype* object)
{
    m_ComponentCount = 0;
    Engine::SetMemory( &m_ComponentBits[0], 0, 4 * DV_COMPONENT_DWORD_COUNT_PER_COMPONENT_COUNT );
}