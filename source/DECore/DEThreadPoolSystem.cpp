#include "../../include/DECore.hpp"

std::atomic<DvigEngine::debool> DvigEngine::ThreadPoolSystem::m_IsRunning;
DvigEngine::deint32 DvigEngine::ThreadPoolSystem::m_ThreadCursor;
DvigEngine::ThreadPoolThreadData DvigEngine::ThreadPoolSystem::m_ThreadQueueData[] = {};

void DvigEngine::ThreadPoolSystem::Init()
{
    DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();
    const deisize requestedThreadCount = engine->GetData()->m_RequestedThreadCount;

    DvigEngine::ThreadPoolSystem::m_IsRunning.store(DV_TRUE);
    DvigEngine::ThreadPoolSystem::m_ThreadCursor = 0;
    for (deint32 i = 0; i < requestedThreadCount; ++i)
    {
        DvigEngine::ThreadPoolSystem::m_ThreadQueueData[i].m_Thread = std::thread(
            &DvigEngine::ThreadPoolSystem::DoJobs, i
        );
        DvigEngine::ThreadPoolSystem::m_ThreadQueueData[i].m_JobCount = 0;
    }
}

void DvigEngine::ThreadPoolSystem::AddJob(deint32 threadIndex, depjob job, void* arguments, const deusize argumentCount)
{
    // Get supported CPU thread count
    DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();
    const deisize requestedThreadCount = engine->GetData()->m_RequestedThreadCount;
    if (DvigEngine::ThreadPoolSystem::m_ThreadCursor >= requestedThreadCount) {
        DvigEngine::ThreadPoolSystem::m_ThreadCursor = 0;
    }

    // Assign thread index
    deint32 curThreadIndex = threadIndex;
    if (curThreadIndex == (deint32)DV_NULL || curThreadIndex < 0 || curThreadIndex >= requestedThreadCount) {
        curThreadIndex = DvigEngine::ThreadPoolSystem::m_ThreadCursor;
        // Increment thread cursor
        DvigEngine::ThreadPoolSystem::m_ThreadCursor += 1;
    }

    // Check if job count per thread
    // exceeded maximum value
    if (DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount >= DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT) {
        return;
    }

    const deint32 jobIndex = DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount;
    const demachword* curArguments = (const demachword*)arguments;
    DvigEngine::Engine::MemoryCopy(
        &DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_Jobs[jobIndex].m_Arguments[0],
        &curArguments[0],
        sizeof(DvigEngine::demachword) * argumentCount
    );
    DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_Jobs[jobIndex].m_pJob = job;
    DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount += 1;
}

void DvigEngine::ThreadPoolSystem::AddJobArray(deint32 threadIndex, depjob* jobs, const deusize jobCount, void* arguments, const deusize* argumentCounts)
{
    // Get supported CPU thread count
    DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();
    const deisize requestedThreadCount = engine->GetData()->m_RequestedThreadCount;
    if (DvigEngine::ThreadPoolSystem::m_ThreadCursor >= requestedThreadCount) {
        DvigEngine::ThreadPoolSystem::m_ThreadCursor = 0;
    }

    // Assign thread index
    deint32 curThreadIndex = threadIndex;
    if (curThreadIndex == (deint32)DV_NULL || curThreadIndex < 0 || curThreadIndex >= requestedThreadCount) {
        curThreadIndex = DvigEngine::ThreadPoolSystem::m_ThreadCursor;
    }

    // Check if job count per thread
    // exceeded maximum value
    if (DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount >= DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT) {
        return;
    }

    deint32 jobIndex;
    demachword* argumentsOffsetAddress = (demachword*)arguments;
    for (deint32 i = 0; i < (deint32)jobCount; ++i)
    {
        jobIndex = DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount;

        // Copy arguments
        DvigEngine::Engine::MemoryCopy(
            &DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_Jobs[jobIndex].m_Arguments[0],
            &argumentsOffsetAddress[0],
            sizeof(DvigEngine::demachword) * argumentCounts[i]
        );

        // Assign job
        DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_Jobs[jobIndex].m_pJob = jobs[i];
        DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount += 1;

        // Increase arguments offset
        argumentsOffsetAddress = DvigEngine::Ptr<demachword*>::Add( &argumentsOffsetAddress, sizeof(demachword) * argumentCounts[i] );
        
        // Increment job index
        jobIndex += 1;
        if (DvigEngine::ThreadPoolSystem::m_ThreadQueueData[curThreadIndex].m_JobCount >= DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT) {
            return;
        }

        // Increment thread index
        if (threadIndex == (deint32)DV_NULL || threadIndex < 0 || threadIndex >= requestedThreadCount) {
            // Check
            DvigEngine::ThreadPoolSystem::m_ThreadCursor += 1;
            if (DvigEngine::ThreadPoolSystem::m_ThreadCursor >= requestedThreadCount) {
                DvigEngine::ThreadPoolSystem::m_ThreadCursor = 0;
            }
            curThreadIndex = DvigEngine::ThreadPoolSystem::m_ThreadCursor;
        }
    }
}

void DvigEngine::ThreadPoolSystem::DoJobs(deint32 threadIndex)
{
    while (DvigEngine::ThreadPoolSystem::m_IsRunning.load() == DV_TRUE)
    {
        if (DvigEngine::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_JobCount == 0) {
            continue;
        }

        for (deint32 i = 0; i < (deint32)DvigEngine::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_JobCount; ++i)
        {
            DvigEngine::ThreadPoolSystem::m_ThreadQueueData[i].m_IsRunning.store(DV_TRUE);
            demachword* jobArguments = &DvigEngine::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_Jobs[i].m_Arguments[0];
            DvigEngine::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_Jobs[i].m_pJob(
                &jobArguments[0], threadIndex
            );
            DvigEngine::ThreadPoolSystem::m_ThreadQueueData[i].m_IsRunning.store(DV_FALSE);
            DvigEngine::ThreadPoolSystem::m_ThreadQueueData[threadIndex].m_JobCount -= 1;
        }
    }
}

void DvigEngine::ThreadPoolSystem::WaitForJobs()
{
    DvigEngine::Engine* engine = DvigEngine::Engine::GetClassInstance();
    const deisize requestedThreadCount = (const deisize)engine->GetData()->m_RequestedThreadCount;

    for (deint32 i = 0; i < requestedThreadCount; ++i) {
        while (DvigEngine::ThreadPoolSystem::m_ThreadQueueData[i].m_IsRunning.load() == DV_TRUE);
        DvigEngine::ThreadPoolSystem::m_ThreadQueueData[i].m_Thread.join();
    }
}

void DvigEngine::ThreadPoolSystem::Terminate()
{
    auto lambdaCall = [](demachword* arguments, deint32 jobIndex) {
        DvigEngine::ThreadPoolSystem::m_IsRunning.store(DV_FALSE | (jobIndex + arguments[0] == 0 ? 0 : 1));
    };
    DvigEngine::ThreadPoolSystem::AddJob( 0, lambdaCall, nullptr, 0 );
}