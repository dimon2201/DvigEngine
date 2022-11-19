#include "../../include/DECore.hpp"

void DvigEngine::ICommon::Free()
{
    m_Engine->Delete( this->m_MemoryObject );
}

void DvigEngine::ICommon::SetUSIDAndUIIDAndMemoryObjectAndEngine(deuchar* USID, const demachint UIID, MemoryObject* memoryObject, Engine* engine)
{
    deisize m_USIDByteWidth = 0;
    while (USID[m_USIDByteWidth] != 0) { this->m_USID[m_USIDByteWidth] = USID[m_USIDByteWidth]; ++m_USIDByteWidth; }
    this->m_USID[m_USIDByteWidth] = 0;
    this->m_UIID = UIID;

    this->m_MemoryObject = memoryObject;
    this->m_Engine = engine;
}