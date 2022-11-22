#include "../../include/DECore.hpp"
#include "../../include/DvigEngineMacros.hpp"

DvigEngine::deusize DvigEngine::String::CharactersCount(const destring op1)
{
    deuchar* op1Copy = (deuchar*)op1;
    while (*++op1Copy != 0);
    const deusize op1ByteWidth = (deusize)(op1Copy - (deuchar*)op1);
    
    return op1ByteWidth;
}

DvigEngine::deresult DvigEngine::String::CompareCharacters(const char* op1, const char* op2, const deusize opByteWidth)
{
    // deint32 cycle = 0;
    // while (op1[cycle] == op2[cycle] && ++cycle < opByteWidth);
    // std::cout << cycle << " " << opByteWidth << std::endl;
    // if (cycle == opByteWidth) {
    //     return DV_TRUE;
    // } else {
    //     return DV_FALSE;
    // }

    deint32 cycle = 0;
    while (op1[cycle] == op2[cycle] && ++cycle < opByteWidth);
    
    if (cycle == opByteWidth) {
        return DV_TRUE;
    } else {
        return DV_FALSE;
    }
}

DvigEngine::deresult DvigEngine::String::CompareCharacterStrings(const destring op1, const destring op2)
{
    const deusize op1ByteWidth = String::CharactersCount(op1);
    const deusize op2ByteWidth = String::CharactersCount(op2);

    deusize cycle = 0;
    if (op1ByteWidth != op2ByteWidth) { return DV_FALSE; }
    while (op1[cycle] == op2[cycle] && ++cycle < op1ByteWidth);

    if (cycle == op1ByteWidth) {
        return DV_TRUE;
    } else {
        return DV_FALSE;
    }
}

DvigEngine::MemoryObject* DvigEngine::String::ConcateCharacters(destring op1, destring op2)
{
    const deusize op1ByteWidth = String::CharactersCount(op1);
    const deusize op2ByteWidth = String::CharactersCount(op2);

    MemoryObject* memoryObject = Engine::Allocate(0, op1ByteWidth + op2ByteWidth);
    deuchar* chars = (deuchar*)memoryObject->GetAddress();
    
    DvigEngine::Engine::MemoryCopy(&chars[0], &op1[0], op1ByteWidth);
    DvigEngine::Engine::MemoryCopy(&chars[op1ByteWidth], &op2[0], op2ByteWidth);

    return memoryObject;
}