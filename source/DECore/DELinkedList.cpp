#include "../../include/DECore.hpp"

void DvigEngine::LINKED_LIST_DATA::Init(Engine* engine, LinkedList* object)
{
    
}

void DvigEngine::LinkedList::Init(const DvigEngine::deusize entryValueByteWidth)
{
    m_Data.m_EntryCount = 0;
    m_Data.m_EntryByteWidth = sizeof(LINKED_LIST_DATA_ENTRY) + sizeof(MemoryObject) + entryValueByteWidth;
    m_Data.m_EntryValueByteWidth = entryValueByteWidth;
}

DvigEngine::deint32 DvigEngine::LinkedList::Insert(void* const value)
{
    LINKED_LIST_DATA_ENTRY* newEntry = MakeEntry(value);

    newEntry->m_PrevAddress = m_Data.m_Head;
    m_Data.m_Head = newEntry;

    return m_Data.m_EntryCount++;
}

void DvigEngine::LinkedList::Replace(const deint32 index, void* const value)
{
    deint32 cycle = 0;
    deint32 indexOfInterest = m_Data.m_EntryCount - index - 1;
    LINKED_LIST_DATA_ENTRY* entry = m_Data.m_Head;
    while (entry->m_PrevAddress != nullptr && cycle++ < indexOfInterest)
    {
        entry = entry->m_PrevAddress;
    }

    Engine::CopyMemory(entry->m_Value->GetAddress(), value, m_Data.m_EntryValueByteWidth);
}

void* DvigEngine::LinkedList::Find(const deint32 index)
{
    deint32 cycle = 0;
    deint32 indexOfInterest = m_Data.m_EntryCount - index - 1;
    LINKED_LIST_DATA_ENTRY* entry = m_Data.m_Head;
    while (entry->m_PrevAddress != nullptr && cycle++ < indexOfInterest)
    {
        entry = entry->m_PrevAddress;
    }

    return entry->m_Value;
}

DvigEngine::LINKED_LIST_DATA_ENTRY* DvigEngine::LinkedList::MakeEntry(void* const value)
{
    // MemoryObject* entryValue = Engine::ObjectAllocate(0, m_Data.m_EntryValueByteWidth);
    // Engine::CopyMemory(entryValue->GetData()->m_Address, value, m_Data.m_EntryValueByteWidth);

    LINKED_LIST_DATA_ENTRY* newEntry = (LINKED_LIST_DATA_ENTRY*)Engine::ObjectAllocate(1, m_Data.m_EntryByteWidth);
    newEntry->m_Value = (MemoryObject*)value;

    return newEntry;
}