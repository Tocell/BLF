#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <atomic>
#include <cstdint>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    explicit ThreadPool(int8_t thread_cnt);
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... arg)
        ->std::future<std::invoke_result_t<F, Args...>>;

    bool joinThread();

private:
    std::atomic<bool> is_running_;
    std::vector<std::thread> threads_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<std::function<void()>> task_queue_;
};

inline ThreadPool::ThreadPool(int8_t thread_cnt) : is_running_(true)
{
    threads_.reserve(thread_cnt);

    for (int8_t i = 0; i < thread_cnt; ++i)
    {
        threads_.emplace_back([this]
        {
            while (is_running_.load())
            {
                std::function<void()> task;
                {
                    std::unique_lock lock(mutex_);
                    condition_.wait(lock,
                        [this] { return !is_running_ || !task_queue_.empty(); });
                    if (!is_running_ && task_queue_.empty())
                        return;
                    task = std::move(task_queue_.front());
                    task_queue_.pop();
                }
                task();
            }
        });
    }
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... arg)
    ->std::future<std::invoke_result_t<F, Args...>>
{
    using return_type = std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(arg)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock(mutex_);
        if (!is_running_)
            throw std::runtime_error("enqueue on non-running ThreadPool");
        task_queue_.emplace([task = std::move(task)]{ (*task)(); });
    }
    condition_.notify_one();
    return res;
}

inline bool ThreadPool::joinThread()
{
    is_running_.store(false);

    condition_.notify_all();
    for (auto& thread : threads_)
    {
        if (thread.joinable())
            thread.join();
    }

    return true;
}

inline ThreadPool::~ThreadPool()
{
    joinThread();
}

#endif //THREADPOOL_H