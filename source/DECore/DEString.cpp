#include "../../include/DECore.hpp"
#include "../../include/DvigEngineMacros.hpp"

DvigEngine::STRING_DATA::STRING_DATA(const char* str)
{
    deuchar* strDE = (deuchar*)str;
    deuchar* strCopy = (deuchar*)str;
    while (*strCopy != 0) { m_Chars[deint32(strCopy - strDE)] = str[deint32(strCopy - strDE)]; strCopy++; };
    m_Chars[deint32(strCopy - strDE)] = 0;
    m_ByteWidth = (deusize)(strCopy - strDE);
}

void DvigEngine::STRING_DATA::Init(Engine* engine, String* object)
{

}

DvigEngine::deusize DvigEngine::String::CharactersCount(const destring op1)
{
    deuchar* op1Copy = (deuchar*)op1;
    while (*++op1Copy != 0);
    const deusize op1ByteWidth = (deusize)(op1Copy - (deuchar*)op1);
    
    return op1ByteWidth;
}

DvigEngine::deresult DvigEngine::String::Compare(STRING_DATA* op1, STRING_DATA* op2)
{
    deuint32 op1StrByteWidth = op1->m_ByteWidth;
    deuint32 op2StrByteWidth = op2->m_ByteWidth;

    if (op1StrByteWidth != op2StrByteWidth) { return DV_FALSE; }

    for (deisize i = 0; i < op1StrByteWidth; ++i)
    {
        if (op1->m_Chars[i] != op2->m_Chars[i])
        {
            return DV_FALSE;
        }
    }

    return DV_TRUE;
}

DvigEngine::deresult DvigEngine::String::CompareCharacters(const destring op1, const destring op2)
{
    const deusize op1ByteWidth = String::CharactersCount(op1);
    const deusize op2ByteWidth = String::CharactersCount(op2);

    deusize cycle = 0;
    if (op1ByteWidth != op2ByteWidth) { return DV_FALSE; }
    while (op1[cycle] == op2[cycle] && cycle++ < op1ByteWidth);

    if (cycle - 1 == op1ByteWidth) {
        return DV_TRUE;
    } else {
        return DV_FALSE;
    }
}

DvigEngine::MemoryObject* DvigEngine::String::ConcateCharacters(destring op1, destring op2)
{
    const deusize op1ByteWidth = String::CharactersCount(op1);
    const deusize op2ByteWidth = String::CharactersCount(op2);

    MemoryObject* memoryObject = Engine::ObjectAllocate(0, op1ByteWidth + op2ByteWidth);
    deuchar* chars = (deuchar*)memoryObject->GetAddress();
    
    Engine::CopyMemory(chars, &op1[0], op1ByteWidth);
    Engine::CopyMemory(&chars[op1ByteWidth], &op2[0], op2ByteWidth);

    return memoryObject;
}