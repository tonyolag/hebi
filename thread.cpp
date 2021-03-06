#include "thread.hpp"

namespace thread
{
    std::unordered_map<std::thread::id, std::string> names;
    std::mutex mutex;

    std::string get_current_name()
    {
        std::thread::id tid = std::this_thread::get_id();
        std::lock_guard<std::mutex> lock(mutex);
        return names[tid];
    }

    void rename_current(std::string pName)
    {
        std::thread::id tid = std::this_thread::get_id();
        std::lock_guard<std::mutex> lock(mutex);
        names[tid] = pName;
    }
    
    void init()
    {
        std::thread::id tid = std::this_thread::get_id();
        std::lock_guard<std::mutex> lock(mutex);
        names[tid] = std::string("main");
    }
}
