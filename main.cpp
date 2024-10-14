#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
class Semaphore
{ 
public:
Semaphore(int count = 0) : count_(count) {} 
void wait() 
{
  std::unique_lock<std::mutex> lock(mtx_);
  cv_.wait(lock, [this]() { return count_ > 0; }); --count_; 
} 

void signal()
{
  std::unique_lock<std::mutex> lock(mtx_);
  ++count_; 
  cv_.notify_one();
} 
private: 
std::mutex mtx_;
std::condition_variable cv_;
int count_; 
}; 
Semaphore sem(2); // Semaphore initialized to 2 (allows up to 2 threads) void access_resource(int thread_id) { sem.wait(); // Wait to acquire the semaphore (decrement counter) std::cout << "Thread " << thread_id << " is accessing the resource" << std::endl; std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate resource usage std::cout << "Thread " << thread_id << " is releasing the resource" << std::endl; sem.signal(); // Signal (release) the semaphore (increment counter) } 

int main() 
{ 
  std::thread t1(access_resource, 1);
  std::thread t2(access_resource, 2);
  std::thread t3(access_resource, 3);
  t1.join();
  t2.join();
  t3.join(); 
  return 0; 
}
