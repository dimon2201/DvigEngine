#include "../../include/DvigEngine.hpp"
#include <iostream>
#include <functional>

void DvigEngine::JobQueue::Push(dvcallback callback, void* argumentMemory, const dvusize argumentCount)
{    
    if (m_Data.m_JobCount >= DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT) { return; }

    m_Data.m_StopFlag.store(DV_FALSE);
    m_Data.m_ReturnFlag.store(DV_FALSE);
    
    Engine::CopyMemory(&m_Data.m_JobArguments[DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * m_Data.m_JobCount], argumentMemory, argumentCount * sizeof(dvmachword));
    m_Data.m_Jobs[m_Data.m_JobCount++] = callback;
}

void DvigEngine::JobQueue::Start()
{
    while (m_Data.m_StopFlag.load() == DV_FALSE)
    {
        if (m_Data.m_JobCount == 0)
        {
            m_Data.m_ReturnFlag.store(DV_TRUE);
            continue;
        }
        m_Data.m_JobCount--;

        dvmachword* jobArguments = &m_Data.m_JobArguments[ DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * m_Data.m_JobCount ];
        m_Data.m_ReturnFlag.store(DV_FALSE);
        m_Data.m_Jobs[m_Data.m_JobCount]( &jobArguments[0], m_Data.m_JobCount );
    }
}

void DvigEngine::JobQueue::Stop()
{
    while (m_Data.m_ReturnFlag.load() == DV_FALSE);
    m_Data.m_StopFlag.store(DV_TRUE);
}