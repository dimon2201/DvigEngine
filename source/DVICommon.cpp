#include "../include/DvigEngine.hpp"

DvigEngine::ICommon::ICommon(const char* stringID)
{
    dvusize byteWidth = 0;
    while (stringID[++byteWidth] != 0);
    DvigEngine::Engine::CopyMemory((void*)&m_SID[0], (void*)&stringID[0], byteWidth);
}