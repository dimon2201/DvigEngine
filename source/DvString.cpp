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