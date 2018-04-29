#ifndef TSP_THREADPOOL
#define TSP_THREADPOOL

#include <atomic>
#include <functional>
#include <pthread.h>
#include <queue>
#include <thread>
#include <vector>

using std::vector;

class TSPThreadPoolManager {
public:
  TSPThreadPoolManager(vector<std::thread> &t) : threads(t) {}
  ~TSPThreadPoolManager() {
    for (size_t i = 0; i < threads.size(); i++) {
      if (threads[i].joinable())
        threads[i].join();
    }
  }

private:
  TSPThreadPoolManager(const TSPThreadPoolManager &) = delete;
  TSPThreadPoolManager &operator=(const TSPThreadPoolManager &) = delete;
  vector<std::thread> &threads;
};

class TSPThreadPool {
public:
  typedef std::function<void()> task_t;

  explicit TSPThreadPool(int n = 0);
  ~TSPThreadPool();

private:
  TSPThreadPool(const TSPThreadPool &) = delete;
  TSPThreadPool &operator=(const TSPThreadPool &) = delete;

  std::atomic<bool> stop;

  std::queue<task_t> tasks;
  std::vector<std::thread> threads;
  TSPThreadPoolManager tpm;
};

#endif
