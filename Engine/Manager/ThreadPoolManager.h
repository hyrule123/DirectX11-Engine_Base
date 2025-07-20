#pragma once
#include <Engine/Util/Singleton.h>

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace core
{
    class ThreadPoolManager
    {
        friend class GameEngine;

        DECLARE_SINGLETON(ThreadPoolManager);
    private:
        ThreadPoolManager();
        void init();

        ~ThreadPoolManager();
        
    public:
        // job 을 추가한다.
        template <class F, class... Args>
        std::future<typename std::invoke_result<F, Args...>::type> enquque_job(F&& f, Args&&... args);

    private:
        
        void init(size_t _numThread);

    private:
        // Worker 쓰레드
        void worker_thread();

    private:
        // 총 Worker 쓰레드의 개수.
        size_t m_threads_count;
        // Worker 쓰레드를 보관하는 벡터.
        std::vector<std::thread> m_threads;
        // 할일들을 보관하는 job 큐.
        std::queue<std::function<void()>> m_jobs;
        // 위의 job 큐를 위한 cv 와 m.
        std::condition_variable m_condition_var_job_queue;
        std::mutex m_mtx_job_queue;

        // 모든 쓰레드 종료
        bool m_b_stop_all;
    };


    template <class F, class... Args>
    std::future<typename std::invoke_result<F, Args...>::type>
        ThreadPoolManager::enquque_job(F&& f, Args&&... args)
    {
        if (m_b_stop_all) {
            throw std::runtime_error("ThreadPoolManager 사용 중지됨");
        }

        using return_type = std::invoke_result<F, Args...>::type;

        auto job = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> job_result_future = job->get_future();
        {
            std::lock_guard<std::mutex> lock(m_mtx_job_queue);
            m_jobs.push([job]() { (*job)(); });
        }
        m_condition_var_job_queue.notify_one();

        return job_result_future;
    }
}
