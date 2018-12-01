#ifndef PRODUCENTCONSUMER_THREADPOOL_H
#define PRODUCENTCONSUMER_THREADPOOL_H

#include <list>
#include <thread>
#include <vector>
#include <iostream>
#include <condition_variable>

template<typename job_t, typename worker_t>
class ThreadPool {
private:
    std::list<job_t> queue;

    std::mutex qmutex;
    std::condition_variable wtr;

    std::vector<std::thread> threads;
    const worker_t & worker;
public:
    ThreadPool(const unsigned int num_threads, const worker_t & worker);
    void process(const job_t job);
    void join();
};


template<typename job_t, typename worker_t>
ThreadPool<job_t, worker_t>::ThreadPool(const unsigned int num_threads, const worker_t &worker) : worker(worker) {
    threads.resize(num_threads);
    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread([&] () {
            job_t task;
            std::unique_lock<std::mutex> lock(qmutex, std::defer_lock);
            do {
                lock.lock();
                if (queue.empty()) wtr.wait(lock, [this]{ return !this->queue.empty(); });
                task = queue.front();
                queue.pop_front();
                lock.unlock();
                if(task != 0) worker(task);  // Na zaver zavolame "worker" funkci, ktera ulohu vykona
            } while(task != 0);              // Pokud je "zpracovana" uloha 0, skoncime
        });
    }
}

template<typename job_t, typename worker_t>
void ThreadPool<job_t, worker_t>::process(const job_t job) {
    qmutex.lock();
    queue.push_back(std::move(job));
    qmutex.unlock();
    wtr.notify_one();
}

template<typename job_t, typename worker_t>
void ThreadPool<job_t, worker_t>::join() {
    for(unsigned int i = 0 ; i < threads.size() ; i++) threads[i].join();
}

#endif //PRODUCENTCONSUMER_THREADPOOL_H
