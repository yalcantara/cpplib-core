
#include <ylib/logging/Logger.h>

using namespace ylib::logging;

// Simple use case of the Logger class
Logger log = Logger::get("main");

int main(){


    std::string val = sfput("pop ${}", "sample");
    println(val);

    LocalDate date;
    println(date.toString());
    log.info("An info message");
    log.warn("A warning message");
    log.error("An error message");
}
