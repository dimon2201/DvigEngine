#include "../include/DvigEngine.hpp"
#include "../include/DvigEngineMacros.hpp"

DvigEngine::STRING_DATA::STRING_DATA(const char* str)
{
    dvusize strByteWidth = 0;
    dvuchar* strDE = (dvuchar*)str;
    dvuchar* strCopy = (dvuchar*)str;
    while (*strCopy != 0 && !(m_Chars[dvint32(strCopy - strDE) + 1] = 0)) { m_Chars[dvint32(strCopy - strDE)] = str[dvint32(strCopy - strDE)]; strCopy++; };
    m_ByteWidth = (dvusize)(strCopy - strDE);
}

DvigEngine::dvusize DvigEngine::String::CharactersCount(dvstring op1)
{
    dvuchar* op1Copy = (dvuchar*)op1;
    while (*++op1Copy != 0);
    const dvusize op1ByteWidth = (dvusize)(op1Copy - (dvuchar*)op1);
    
    return op1ByteWidth;
}

DvigEngine::dvresult DvigEngine::String::Compare(STRING_DATA* op1, STRING_DATA* op2)
{
    dvuint32 op1StrByteWidth = op1->m_ByteWidth;
    dvuint32 op2StrByteWidth = op2->m_ByteWidth;

    if (op1StrByteWidth != op2StrByteWidth) { return DV_FALSE; }

    dvresult result = 0;
    for (dvisize i = 0; i < op1StrByteWidth; ++i)
    {
        if (op1->m_Chars[i] != op2->m_Chars[i])
        {
            return DV_FALSE;
        }
    }

    return DV_TRUE;
}

DvigEngine::dvresult DvigEngine::String::CompareCharacters(dvstring op1, dvstring op2)
{
    const dvusize op1ByteWidth = String::CharactersCount(op1);
    const dvusize op2ByteWidth = String::CharactersCount(op2);

    dvisize cursor = 0;
    if (op1ByteWidth != op2ByteWidth) { return DV_FALSE; }
    while (op1[cursor] == op2[cursor] && cursor++ < op1ByteWidth);

    if (cursor == op1ByteWidth)
    {
        return DV_TRUE;
    }
    else
    {
        return DV_FALSE;
    }
}

DvigEngine::MemoryObject* DvigEngine::String::ConcateCharacters(dvstring op1, dvstring op2)
{
    const dvusize op1ByteWidth = String::CharactersCount(op1);
    const dvusize op2ByteWidth = String::CharactersCount(op2);

    MemoryObject* memoryObject = Engine::AllocateObject(0, op1ByteWidth + op2ByteWidth);
    dvuchar* chars = (dvuchar*)memoryObject->GetData()->m_Address;
    
    Engine::CopyMemory(chars, &op1[0], op1ByteWidth);
    Engine::CopyMemory(&chars[op1ByteWidth], &op2[0], op2ByteWidth);

    return memoryObject;
}