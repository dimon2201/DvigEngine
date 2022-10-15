#include "../include/DvigEngine.hpp"

void DvigEngine::IObject::SetCreateeAndMemoryObject(IObject** createe, MemoryObject* memoryObject)
{
    m_Createe = createe;
    m_MemoryObject = memoryObject;
}