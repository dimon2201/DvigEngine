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

void DvigEngine::LinkedList::Replace(const dvint32 index, MemoryObject* const value)
{
    dvint32 cycle = 0;
    dvint32 indexOfInterest = m_Data.m_EntryCount - index - 1;
    LINKED_LIST_DATA_ENTRY* entry = m_Data.m_Head;
    while (entry->m_PrevAddress != nullptr && cycle++ < indexOfInterest)
    {
        entry = entry->m_PrevAddress;
    }

    entry->m_Value = value;
}

DvigEngine::MemoryObject* DvigEngine::LinkedList::Find(const dvint32 index)
{
    dvint32 cycle = 0;
    dvint32 indexOfInterest = m_Data.m_EntryCount - index - 1;
    LINKED_LIST_DATA_ENTRY* entry = m_Data.m_Head;
    while (entry->m_PrevAddress != nullptr && cycle++ < indexOfInterest)
    {
        entry = entry->m_PrevAddress;
    }

    return entry->m_Value;
}

DvigEngine::dvint32 DvigEngine::LinkedList::FindValue(void* value)
{
    dvint32 cycle = 0;
    dvint32 entryCount = m_Data.m_EntryCount;
    LINKED_LIST_DATA_ENTRY* entry = m_Data.m_Head;
    while (entry->m_PrevAddress != nullptr && cycle++ < entryCount)
    {
        if (entry->m_Value == value) { return cycle - 1; }
        entry = entry->m_PrevAddress;
    }

    return DV_NULL;
}

DvigEngine::LINKED_LIST_DATA_ENTRY* DvigEngine::LinkedList::MakeEntry(MemoryObject* const value)
{
    LINKED_LIST_DATA_ENTRY* newEntry = (LINKED_LIST_DATA_ENTRY*)Engine::AllocateObject(0, m_Data.m_EntryByteWidth);
    newEntry->m_Value = value;

    return newEntry;
}