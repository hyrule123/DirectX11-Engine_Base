#pragma once
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ehw
{
    class ThreadPoolManager 
    {
        friend class GameEngine;
    public:
        // job 을 추가한다.
        template <class F, class... Args>
        static std::future<typename std::invoke_result<F, Args...>::type> EnqueueJob(F&& f, Args&&... args);

    private:
        static void Init(size_t _numThread);
        static void Release();

    private:
        // Worker 쓰레드
        static void WorkerThread();

    private:
        // 총 Worker 쓰레드의 개수.
        static size_t mNumThread;
        // Worker 쓰레드를 보관하는 벡터.
        static std::vector<std::thread> mWorkerThreads;
        // 할일들을 보관하는 job 큐.
        static std::queue<std::function<void()>> mJobs;
        // 위의 job 큐를 위한 cv 와 m.
        static std::condition_variable mCVJobqueue;
        static std::mutex mMtxJobQueue;

        // 모든 쓰레드 종료
        static bool mStopAll;



    private:
        ThreadPoolManager() = delete;
        ~ThreadPoolManager() = delete;
    };


    template <class F, class... Args>
    std::future<typename std::invoke_result<F, Args...>::type>
        ThreadPoolManager::EnqueueJob(F&& f, Args&&... args)
    {
        if (mStopAll) {
            throw std::runtime_error("ThreadPoolManager 사용 중지됨");
        }

        using return_type = std::invoke_result<F, Args...>::type;

        auto job = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> job_result_future = job->get_future();
        {
            std::lock_guard<std::mutex> lock(mMtxJobQueue);
            mJobs.push([job]() { (*job)(); });
        }
        mCVJobqueue.notify_one();

        return job_result_future;
    }
}
