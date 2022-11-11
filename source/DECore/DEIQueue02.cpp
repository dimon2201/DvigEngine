#include "../../include/DECore.hpp"

std::thread DvigEngine2::IQueue::m_Threads[] = {};

void DvigEngine2::IQueue::Init()
{
    this->m_JobCount = 0;
    for (deint32 i = 0; i < DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT * DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT; ++i) {
        this->m_JobArguments[i] = 0;
    }
    for (deint32 i = 0; i < DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT; ++i) {
        this->m_Jobs[i] = nullptr;
    }
}

void DvigEngine2::IQueue::AddJob(decallback callback, void* arguments, const deusize argumentCount)
{
    if (this->m_JobCount >= DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT) { return; }
    if (argumentCount >= DV_MAX_JOB_QUEUE_THREAD_JOB_ARGUMENT_COUNT) { return; }

    const demachword* mwArguments = (const demachword*)arguments;
    DvigEngine2::Engine::CopyMemory( &this->m_JobArguments[0], &mwArguments[0], sizeof(demachword) * argumentCount );
    this->m_Jobs[this->m_JobCount] = callback;
    this->m_JobCount += 1;
}

void DvigEngine2::IQueue::DoJobs()
{
    for (deint32 i = 0; i < DV_MAX_JOB_QUEUE_THREAD_JOB_COUNT; ++i) {
        if (this->m_Jobs[i] != nullptr) {
            this->m_Jobs[i]( &this->m_JobArguments[0], 0 );
        }
    }
}