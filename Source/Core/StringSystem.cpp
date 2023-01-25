#include "../../Include/DvigEngine/Core.hpp"

DvigEngine::usize DvigEngine::StringSystem::GetByteWidth(const char* string)
{
    sint32 cycle = 0;
    while (string[cycle] != 0) { cycle += 1; }

    return (usize)cycle;
}

DvigEngine::boolean DvigEngine::StringSystem::GetStringsEqual(const char* string1, const char* string2)
{
    usize string1ByteWidth = StringSystem::GetByteWidth( &string1[0] );
    usize string2ByteWidth = StringSystem::GetByteWidth( &string2[0] );
    if (string1ByteWidth != string2ByteWidth) {
        return DVIG_FALSE;
    }

    sint32 cycle = 0;
    while (string1[cycle] == string2[cycle] && (usize)++cycle < string1ByteWidth);

    if ((usize)cycle == string1ByteWidth) {
        return DVIG_TRUE;
    } else {
        return DVIG_FALSE;
    }
}