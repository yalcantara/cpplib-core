
#include <ylib/logging/Logger.h>

using namespace ylib::logging;

// Simple use case of the Logger class
Logger logger = Logger::get("main");

int main() {
    logger.info("Hello World from cpplib-core. Your C++ common library!!!");
    return 0;
}
