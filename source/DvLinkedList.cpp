#include "../include/DvigEngine.hpp"

void DvigEngine::LinkedList::Init(const DvigEngine::dvusize capacity)
{
    m_Data.m_EntryCount = capacity;
    m_Data.m_EntryByteWidth = sizeof(LINKED_LIST_DATA_ENTRY);
    m_Data.m_AllocatedEntryCount = capacity;
}

DvigEngine::dvint32 DvigEngine::LinkedList::Insert(MemoryObject* const value)
{
    LINKED_LIST_DATA_ENTRY* newEntry = MakeEntry(value);

    newEntry->m_PrevAddress = m_Data.m_Head;
    m_Data.m_Head = newEntry;

    return m_Data.m_EntryCount++;
}

DvigEngine::MemoryObject* DvigEngine::LinkedList::Find(const dvint32 index)
{
    dvint32 cycle = 0;
    dvint32 indexOfInterest = m_Data.m_EntryCount - index - 1;
    LINKED_LIST_DATA_ENTRY* entry = m_Data.m_Head;
    while (entry->m_PrevAddress != nullptr && cycle++ < indexOfInterest)
    {
        std::cout << cycle << " " << (dvmachword)entry->m_Value << std::endl;
        entry = entry->m_PrevAddress;
    }

    return entry->m_Value;
}

DvigEngine::LINKED_LIST_DATA_ENTRY* DvigEngine::LinkedList::MakeEntry(MemoryObject* const value)
{
    LINKED_LIST_DATA_ENTRY* newEntry = (LINKED_LIST_DATA_ENTRY*)Engine::AllocateObject(0, m_Data.m_EntryByteWidth);
    newEntry->m_Value = value;

    return newEntry;
}