#include "../../include/DECore.hpp"

void DvigEngine2::ICommon::SetSIDAndIIDAndCreateeAndMemoryObjectAndEngine(deuchar* SID, const demachint IID, ICommon** createe, MemoryObject** memoryObject, Engine* engine)
{
    deisize m_SIDByteWidth = 0;
    while (SID[m_SIDByteWidth] != 0) { this->m_SID[m_SIDByteWidth] = SID[m_SIDByteWidth]; ++m_SIDByteWidth; }
    this->m_SID[m_SIDByteWidth] = 0;
    this->m_IID = IID;

    this->m_Createe = createe;
    this->m_MemoryObject = memoryObject;
    this->m_Engine = engine;
}