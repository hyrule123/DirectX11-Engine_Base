
#include "ThreadPoolManager.h"



#include "Engine/Util/AtExit.h"

namespace ehw
{
    ThreadPoolManager::ThreadPoolManager()
        : mNumThread{}
        , mWorkerThreads{}
        , mJobs{}
        , mCVJobqueue{}
        , mMtxJobQueue{}
    {
    }
    ThreadPoolManager::~ThreadPoolManager()
    {
    }
    void ThreadPoolManager::init(size_t _numThread)
    {
        AtExit::AddFunc(std::bind(&ThreadPoolManager::release, this));

        mNumThread = _numThread;
        mWorkerThreads.reserve(mNumThread);
        
        for (size_t i = 0; i < mNumThread; ++i)
        {
            std::wstring threadDesc = L"ThreadPool_000";

            int digits = 1;
            int numThread = (int)i / 10;
            while (numThread != 0)
            {
                numThread /= 10;
                digits += 1;
            }
            for (int i = 0; i < digits; ++i)
            {
                threadDesc.pop_back();
            }
            threadDesc += std::to_wstring(i);
            std::thread thread([this]() { WorkerThread(); });
            SetThreadDescription(thread.native_handle(), threadDesc.c_str());
            mWorkerThreads.emplace_back(std::move(thread));
        }
    }

    void ThreadPoolManager::release()
    {
        mStopAll = true;
        mCVJobqueue.notify_all();

        //큐를 비워준다
        {
            std::unique_lock<std::mutex> lock(mMtxJobQueue);
            std::queue<std::function<void()>> emptyQueue;
            mJobs.swap(emptyQueue);
        }

        for (auto& t : mWorkerThreads)
        {
            t.join();
        }
    }

    void ThreadPoolManager::WorkerThread() 
    {
        while (true) 
        {
            std::unique_lock<std::mutex> lock(mMtxJobQueue);
            mCVJobqueue.wait(lock, [this]() { return false == mJobs.empty() || mStopAll; });
            if (mStopAll && mJobs.empty())
            {
                return;
            }

            // 맨 앞의 job 을 뺀다.
            std::function<void()> job = std::move(mJobs.front());
            mJobs.pop();
            lock.unlock();

            // 해당 job 을 수행한다 :)
            job();
        }
    }


}
