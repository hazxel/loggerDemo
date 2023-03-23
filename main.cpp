#include <iostream>
#include "logger.h"

int main() {
    Logger logger("log");
    for (int i = 0; i < 1000; i++) {
        logger.write("This is a test message");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // logger.stopAndWait();
    logger.stop();
    return 0;
}