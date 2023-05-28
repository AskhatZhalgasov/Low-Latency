#include <cerrno>
#include <cstdint>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

#define PROFILE(message) \
    Profiler profiler(message);

struct Profiler {
    template <class Duration>  
    using sys_time = std::chrono::time_point<std::chrono::system_clock, Duration>;
    using sys_nanos = sys_time<std::chrono::nanoseconds>;

    Profiler(std::string message) : 
        start(std::chrono::system_clock::now()),
        message(message) {}
    
    std::chrono::time_point<std::chrono::system_clock> start;
    std::string message;

    ~Profiler() {
        std::chrono::nanoseconds elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start);
        std::cout << message << ", elapse: " << elapsed.count() << std::endl;
    }
};

int main() {

    // Some questions to speed-up this:
    // 1. Is it memory friendly? 
    // 2. How about cache miss?
    // 3. std::chrono::system_clock kernel space? user space?

    const int maxn = 1e6;

    {
        std::vector<int> vec;
        PROFILE("without reserve");
        for (int it = 0; it < maxn; it++) {
            vec.push_back(it);
        }
        // Result: 44074000
    }
    {
        std::vector<int> vec;
        vec.reserve(maxn);
        PROFILE("with reserve");
        for (int it = 0; it < maxn; it++) {
            vec.push_back(it);
        }
        // Result: 8589590
    }

    return 0;
}