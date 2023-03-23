#include "logger.h"

Logger::Logger(std::string base_filename) : base_filename(base_filename), running(true), accepting(true) {
    current_filename = getCurrentFilename();
    log_file = std::ofstream(current_filename, std::ios::out | std::ios::app);
    write_thread = std::thread(&Logger::writeLogMessages, this);
}


Logger::~Logger() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        running = false;
    }
    cv.notify_one();
    if (write_thread.joinable()) {
        write_thread.join();
    }
    log_file.close();
}

void Logger::write(std::string message) noexcept {
    if (!running || !accepting) {
        return;
    }

    std::thread t([&, message]() noexcept {
        std::lock_guard<std::mutex> lock(queue_mutex);
        log_queue.push(message);
        cv.notify_one();
        
    });
    t.detach();
}

void Logger::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        running = false;
    }
    cv.notify_one();
}

void Logger::stopAndWait() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        accepting = false;
    }
    cv.notify_one();
    write_thread.join();
}


void Logger::writeLogMessages() noexcept {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cv.wait(lock, [this](){ return !log_queue.empty() || !running || !accepting; });
        if (!running || (!accepting && log_queue.empty())) {
            return;
        }
        if (current_filename != getCurrentFilename()) {
            log_file.close();
            current_filename = getCurrentFilename();
            log_file.open(current_filename.c_str(), std::ios::out | std::ios::app);
        }
        std::string message = log_queue.front();
        log_queue.pop();
        lock.unlock();
        time_t now = time(0);
        char* timestamp = ctime(&now);            
        log_file << timestamp << ": " << message << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

std::string Logger::getFilename() {
    return current_filename;
}

std::string Logger::getCurrentFilename() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    std::ostringstream ostr;
    ostr << base_filename << '_' << timeinfo->tm_year + 1900 << timeinfo->tm_mon + 1 << timeinfo->tm_mday << ".log";
    return ostr.str();
}
