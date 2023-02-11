# cpplib-core
A header only commons library for C++, that contains simple to use functions & classes that simplify development.

## Development
- C++ 17
- Docker
- Linux (Ubuntu 18+)
- clang
- cmake

## To compile & run 🔥

The compile and run procesess are automated using <strong>Docker</strong> and some simple scripts. 

```shell
./compile.sh
./run.sh
```

## To use
Start by cloning the repo in a path that is easy to search from your desired project. The cpplib-core library depends on <strong>Boost</strong> and <strong>cpprest</strong> packages, which are easy to install in Ubuntu:

```shell
apt-get install libcpprest-dev
apt-get install libboost-system-dev
```

It has been tested with the clang compiler, with ocacional testing of g++. To add the clang compiler:

```shell
apt-get install clang
```

You can add cpplib to your project by using `cmake`. In your `CMakeLists.txt` file, add the following instructions (asuming `main` is the name of your program):
```
include_directories(<path-to-cpplib/include>)

target_link_libraries(main
        cpprest
        -lstdc++fs
        Boost::boost
        Boost::system
        OpenSSL::SSL)
```

## Examples
### Logging
The `Logger` class simplifies logging by having a simple interface, thread safety, and daily rolling appender. By default, it stores the logs at `./logs` folder (it will be created if doesn't exists).

```cpp
#include <ylib/logging/Logger.h>

using namespace ylib::logging;

// Simple use case of the Logger class
Logger log = Logger::get("main");

int main(){
    log.info("An info message");
    log.warn("A warning message");
    log.error("An error message");
}
```
Output:
```console
INFO   00:21:22.319  [main] - An info message
WARN   00:21:22.327  [main] - A warning message
ERROR  00:21:22.333  [main] - An error message
```

### Language enhancements
Some classes are meant to avoid the pain of of C++ idiosyncrasies. For example de `Bool` class solves the issue of the weak type of C++ bool. It comes with the constant `True` and `False`. Also, the println is overloaded, to simplify printing. The `LocalDate` and `LocalDateTime` is inspired in  the new Java Time Library. The methods `checkParamNotEmpty` & `checkParamBetween`,  helps validate method parameters.

```cpp
Bool is_positive(int num) {
    if (num > 0) {
        return True;
    }

    return False;
}
```

```cpp
int my_function(const char* str){
    checkParamNotEmpty("str", str); //in case of 
    ..
}
```

```cpp
LocalDate date;
println(date.toString());
```