#include "../include/DvigEngine.hpp"

void DvigEngine::CommandQueue::Push(void* argumentMemory, const dvisize argumentCount)
{
    m_Data.m_InstructionCount += argumentCount;
}

void DvigEngine::CommandQueue::Execute()
{
    
}