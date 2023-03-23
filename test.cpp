#include <iostream>
#include <fstream>
#include <string>

#include "logger.h"

void testWrite() {
    Logger logger("log");

    std::string logFilename = logger.getFilename();
    std::ifstream logFile;
    logFile.open(logFilename, std::ios::in);
    logFile.seekg(0, std::ios_base::end);

    std::streampos size = logFile.tellg();

    logger.write("testWrite");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    logFile.seekg(0, std::ios_base::end);

    if (size >= logFile.tellg() && logFilename == logger.getFilename()) {
        std::cout << "assersion failed, log write writing nothing " << std::endl;
    } else {
        std::cout << "write test passed" << std::endl;
    }

}

void testMidNight() {
    // not implemented
}

void testStop() {
    Logger logger("log");

    std::string logFilename = logger.getFilename();
    std::ifstream logFile;
    logFile.open(logFilename, std::ios::in);

    for (int i = 0; i < 100; i++) {
        logger.write("This is a test message");
    }

    logger.stop();

    logFile.seekg(0, std::ios_base::end);
    std::streampos size = logFile.tellg();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    logFile.seekg(0, std::ios_base::end);

    if (size < logFile.tellg() && logFilename == logger.getFilename()) {
        std::cout << "stop test failed, still writing logs after stop() is called." << std::endl;
    } else {
        std::cout << "stop test passed" << std::endl;
    }
}

void testStopAndWait() {
        Logger logger("log");

    std::string logFilename = logger.getFilename();
    std::ifstream logFile;
    logFile.open(logFilename, std::ios::in);

    for (int i = 0; i < 100; i++) {
        logger.write("This is a test message");
    }

    logFile.seekg(0, std::ios_base::end);
    std::streampos size = logFile.tellg();

    logger.stopAndWait();

    logFile.seekg(0, std::ios_base::end);

    if (size >= logFile.tellg() && logFilename == logger.getFilename()) {
        std::cout << "stop-and-wait test failed, not writing queued logs after stopAndWait() is called." << std::endl;
    } else {
        std::cout << "stop-and-wait test passed" << std::endl;
    }
}

int main() {
    testWrite();
    testMidNight();
    testStop();
    testStopAndWait();
    return 0;
}