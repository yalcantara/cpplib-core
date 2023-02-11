
#include <ylib/logging/Logger.h>

using namespace ylib::logging;

// Simple use case of the Logger class
Logger log = Logger::get("main");

int main() {
    log.info("Hello World from cpplib-core. Your C++ common library!!!");
}
