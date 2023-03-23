#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <fstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

class Logger {
    public:
    Logger(std::string base_filename);

    ~Logger();

    void write(std::string message) noexcept;

    void stop();

    void stopAndWait();

    std::string getFilename(); 

private:
    std::string base_filename;
    std::string current_filename;
    std::ofstream log_file;
    std::queue<std::string> log_queue;
    std::thread write_thread;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool running;
    bool accepting;

    void writeLogMessages() noexcept;

    std::string getCurrentFilename();
};

#endif