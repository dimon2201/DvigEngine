#include "../include/DvigEngine.hpp"

void DvigEngine::JobQueue::Push(void* argumentMemory, const dvisize argumentCount)
{
    m_Data.m_JobCount += argumentCount;
}

void DvigEngine::JobQueue::Execute()
{
    
}