
#include "ThreadPoolManager.h"

#include <Engine/Util/AtExit.h>

namespace core
{
    ThreadPoolManager::ThreadPoolManager()
        : m_threads_count{}
        , m_threads{}
        , m_jobs{}
        , m_condition_var_job_queue{}
        , m_mtx_job_queue{}
        , m_b_stop_all{}
    {}
    void ThreadPoolManager::init()
    {
        AtExit::add_func(ThreadPoolManager::destroy_inst);
    }

    ThreadPoolManager::~ThreadPoolManager()
    {
        m_b_stop_all = true;
        m_condition_var_job_queue.notify_all();

        //큐를 비워준다
        {
            std::unique_lock<std::mutex> lock(m_mtx_job_queue);
            std::queue<std::function<void()>> emptyQueue;
            m_jobs.swap(emptyQueue);
        }

        for (auto& t : m_threads)
        {
            t.join();
        }
    }
    void ThreadPoolManager::init(size_t _numThread)
    {
        m_threads_count = _numThread;
        m_threads.reserve(m_threads_count);

        for (size_t i = 0; i < m_threads_count; ++i)
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
            std::thread thread([this]() { worker_thread(); });
            SetThreadDescription(thread.native_handle(), threadDesc.c_str());
            m_threads.emplace_back(std::move(thread));
        }
    }

    void ThreadPoolManager::worker_thread() 
    {
        while (true) 
        {
            std::unique_lock<std::mutex> lock(m_mtx_job_queue);
            m_condition_var_job_queue.wait(lock, [this]() { return false == m_jobs.empty() || m_b_stop_all; });
            if (m_b_stop_all && m_jobs.empty())
            {
                return;
            }

            // 맨 앞의 job 을 뺀다.
            std::function<void()> job = std::move(m_jobs.front());
            m_jobs.pop();
            lock.unlock();

            // 해당 job 을 수행한다 :)
            try {
                job();
            }
            catch (const std::exception& _err) {
                ERROR_MESSAGE_A(_err.what());
            }
        }
    }


}
