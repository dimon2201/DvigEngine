#include "../../include/DECore.hpp"
#include "../../include/DvigEngineMacros.hpp"

DvigEngine2::deusize DvigEngine2::String::CharactersCount(const destring op1)
{
    deuchar* op1Copy = (deuchar*)op1;
    while (*++op1Copy != 0);
    const deusize op1ByteWidth = (deusize)(op1Copy - (deuchar*)op1);
    
    return op1ByteWidth;
}

DvigEngine2::deresult DvigEngine2::String::CompareCharacters(const destring op1, const destring op2, const deusize opByteWidth)
{
    deusize cycle = 0;
    while (op1[cycle] == op2[cycle] && ++cycle < opByteWidth);
    if (cycle == opByteWidth) {
        return DV_TRUE;
    } else {
        return DV_FALSE;
    }
}

DvigEngine2::deresult DvigEngine2::String::CompareCharacterStrings(const destring op1, const destring op2)
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

DvigEngine2::MemoryObject* DvigEngine2::String::ConcateCharacters(destring op1, destring op2)
{
    const deusize op1ByteWidth = String::CharactersCount(op1);
    const deusize op2ByteWidth = String::CharactersCount(op2);

    MemoryObject* memoryObject = Engine::Allocate(0, op1ByteWidth + op2ByteWidth);
    deuchar* chars = (deuchar*)memoryObject->GetAddress();
    
    Engine::CopyMemory(chars, &op1[0], op1ByteWidth);
    Engine::CopyMemory(&chars[op1ByteWidth], &op2[0], op2ByteWidth);

    return memoryObject;
}