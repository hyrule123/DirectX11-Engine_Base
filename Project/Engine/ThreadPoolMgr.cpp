#include "PCH_Engine.h"
#include "ThreadPoolMgr.h"
#include "AtExit.h"

namespace mh
{
    size_t                                  ThreadPoolMgr::mNumThread{};
    std::vector<std::thread>                ThreadPoolMgr::mWorkerThreads{};
    std::queue<std::function<void()>>       ThreadPoolMgr::mJobs{};
    std::condition_variable                 ThreadPoolMgr::mCVJobqueue{};
    std::mutex                              ThreadPoolMgr::mMtxJobQueue{};
    bool                                    ThreadPoolMgr::mStopAll{};


    void ThreadPoolMgr::Init(size_t _numThread)
    {
        AtExit::AddFunc(Release);

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
            std::thread thread([]() { WorkerThread(); });
            SetThreadDescription(thread.native_handle(), threadDesc.c_str());
            mWorkerThreads.emplace_back(std::move(thread));
        }
    }

    void ThreadPoolMgr::Release()
    {
        mStopAll = true;
        mCVJobqueue.notify_all();

        for (auto& t : mWorkerThreads)
        {
            t.join();
        }
    }

    void ThreadPoolMgr::WorkerThread() 
    {
        while (true) 
        {
            std::unique_lock<std::mutex> lock(mMtxJobQueue);
            mCVJobqueue.wait(lock, []() { return !mJobs.empty() || mStopAll; });
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
