#include "../include/DvigEngine.hpp"

void DvigEngine::LinkedList::Init(const DvigEngine::dvusize entryValueByteWidth)
{
    m_Data.m_EntryCount = 0;
    m_Data.m_EntryByteWidth = sizeof(LINKED_LIST_DATA_ENTRY) + sizeof(MemoryObject) + entryValueByteWidth;
    m_Data.m_EntryValueByteWidth = entryValueByteWidth;
}

DvigEngine::dvint32 DvigEngine::LinkedList::Insert(void* const value)
{
    LINKED_LIST_DATA_ENTRY* newEntry = MakeEntry(value);

    newEntry->m_PrevAddress = m_Data.m_Head;
    m_Data.m_Head = newEntry;

    return m_Data.m_EntryCount++;
}

void DvigEngine::LinkedList::Replace(const dvint32 index, void* const value)
{
    dvint32 cycle = 0;
    dvint32 indexOfInterest = m_Data.m_EntryCount - index - 1;
    LINKED_LIST_DATA_ENTRY* entry = m_Data.m_Head;
    while (entry->m_PrevAddress != nullptr && cycle++ < indexOfInterest)
    {
        entry = entry->m_PrevAddress;
    }

    Engine::CopyMemory(entry->m_Value->GetData()->m_Address, value, m_Data.m_EntryValueByteWidth);
}

void* DvigEngine::LinkedList::Find(const dvint32 index)
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

DvigEngine::LINKED_LIST_DATA_ENTRY* DvigEngine::LinkedList::MakeEntry(void* const value)
{
    MemoryObject* entryValue = Engine::AllocateObject(0, m_Data.m_EntryValueByteWidth);
    Engine::CopyMemory(entryValue->GetData()->m_Address, value, m_Data.m_EntryValueByteWidth);

    LINKED_LIST_DATA_ENTRY* newEntry = (LINKED_LIST_DATA_ENTRY*)Engine::AllocateObject(0, m_Data.m_EntryByteWidth);
    newEntry->m_Value = entryValue;

    return newEntry;
}