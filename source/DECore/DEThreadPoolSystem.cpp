#include "../../include/DECore.hpp"

DvigEngine2::debool DvigEngine2::ThreadPoolSystem::m_IsRunning;
DvigEngine2::deint32 DvigEngine2::ThreadPoolSystem::m_ThreadCursor;
DvigEngine2::ThreadPoolThreadData DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[] = {};

void DvigEngine2::ThreadPoolSystem::Init()
{
    DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
    const deusize requestedThreadCount = engine->GetData()->m_RequestedThreadCount;

    DvigEngine2::ThreadPoolSystem::m_IsRunning = DV_TRUE;
    DvigEngine2::ThreadPoolSystem::m_ThreadCursor = 0;
    for (deint32 i = 0; i < requestedThreadCount; ++i)
    {
        DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[i].m_Thread = std::thread(
            &DvigEngine2::ThreadPoolSystem::DoJobs, nullptr, i
        );
        DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[i].m_JobCount = 0;
    }
}

void DvigEngine2::ThreadPoolSystem::AddJob(deint32 threadIndex, depjob job, void* arguments, const deusize argumentCount)
{
    // Get supported CPU thread count
    DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
    const deusize requestedThreadCount = engine->GetData()->m_RequestedThreadCount;
    if (DvigEngine2::ThreadPoolSystem::m_ThreadCursor >= requestedThreadCount) {
        DvigEngine2::ThreadPoolSystem::m_ThreadCursor = 0;
    }

    // Assign thread index
    deint32 curThreadIndex = threadIndex;
    if (curThreadIndex == DV_NULL || curThreadIndex < 0 || curThreadIndex >= requestedThreadCount) {
        curThreadIndex = 0;
    } else {
        curThreadIndex = DvigEngine2::ThreadPoolSystem::m_ThreadCursor;
    }

    // Check if job count per thread
    // exceeded maximum value
    if (DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount >= DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT) {
        return;
    }

    const deint32 jobIndex = DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount;
    const demachword* curArguments = (const demachword*)arguments;
    DvigEngine2::Engine::CopyMemory(
        &DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_Jobs[jobIndex].m_Arguments[0],
        &curArguments[0],
        sizeof(DvigEngine2::demachword) * argumentCount
    );
    DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_Jobs[jobIndex].m_pJob = job;
    DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount += 1;

    // Increment thread cursor
    DvigEngine2::ThreadPoolSystem::m_ThreadCursor += 1;
}

void DvigEngine2::ThreadPoolSystem::DoJobs(demachword* arguments, deint32 threadIndex)
{
    while (DvigEngine2::ThreadPoolSystem::m_IsRunning == DV_TRUE)
    {
        if (DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_JobCount == 0) { continue; }

        for (deint32 i = 0; i < DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_JobCount; ++i)
        {
            demachword* jobArguments = &DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_Jobs[i].m_Arguments[0];
            DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_Jobs[i].m_pJob(
                &jobArguments[0], threadIndex
            );
            DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_JobCount -= 1;
        }
    }
}

void DvigEngine2::ThreadPoolSystem::WaitForJobs()
{
    DvigEngine2::Engine* engine = DvigEngine2::Engine::GetClassInstance();
    const deusize requestedThreadCount = engine->GetData()->m_RequestedThreadCount;

    for (deint32 i = 0; i < requestedThreadCount; ++i) {
        DvigEngine2::ThreadPoolSystem::m_ThreadQueueData[i].m_Thread.join();
    }
}