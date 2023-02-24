# C++ | cpplib-core
A header-only commons library for C++, that contains simple-to-use functions and classes that simplify development.


## Features
- Easy to use
- Header-only & no dependencies!
- String manipulation
- Date and Time utils
- IO utility functions
- Simple Logging mechanism

## Development
- C++ 17
- Docker
- Alpine Linux
- clang
- cmake

## To compile & run 🔥

The compile and run process is automated using **Docker** and some simple scripts.

First, build the image:
```shell
./build-image.sh
```
Then, compile & run:
```shell
./compile.sh
./run.sh
```

This will run the default `main` program which writes a hello message to the console output, as well to the file system as a log. The log file will be created at `Docker_Debug/logs/<current_date>.log`. The `${project}/Docker_Debug` folder maps to `${project}/Debug` in the Docker container.

```console
Docker container created. About to run program main.
INFO   19:33:55.152  [main] - Hello World from cpplib-core. Your C++ common library!!!
```

## Usage
Start by cloning the repo in a path that is easy to search from your desired project. The cpplib-core library has no extra dependencies other than the standard C++ libray.

### As a header
This is the easiest way to add to your project. Include as a header the path `cpplib-core/include`, and compile normally.

### CMake
You can add cpplib-core to your project by using `cmake`. In your `CMakeLists.txt` file, add the following instructions (assuming `main` is the name of your program):
```
include_directories( <path-to-cpplib/include> )

target_link_libraries( main )
```

## Examples
### Logging
The `Logger` class simplifies logging by having a simple interface, thread safety, and daily rolling appender. By default, it stores the logs in `./logs` folder (it will be created if it doesn't exist).

```cpp
#include <ylib/logging/Logger.h>

using namespace ylib::logging;

// Simple use case of the Logger class
Logger log = Logger::get("main");

int main(){
    log.info("An info message");
    log.warn("A warning message");
    log.error("An error message");
    return 0;
}
```
Output:
```console
INFO   00:21:22.319  [main] - An info message
WARN   00:21:22.327  [main] - A warning message
ERROR  00:21:22.333  [main] - An error message
```

### Language enhancements
Some classes are meant to avoid the pain of C++ idiosyncrasies. For example, the `Bool` class solves the issue of the weak type of C++ bool. It comes with the constant `True` and `False`. Also, the `println` function is overloaded, to simplify printing to the standard output. The `LocalDate` and `LocalDateTime` are inspired by the new Java Time Library. The methods `checkParamNotEmpty` & `checkParamBetween`,  helps validate method parameters.

```cpp
// Using the Bool class
Bool is_positive(int num) {
    if (num > 0) {
        return True;
    }

    return False;
}
```

```cpp
int my_function(const char* str){
    checkParamNotEmpty("str", str); // throws Exception if str is null or empty
    ..
}
```

```cpp
LocalDate date;
println(date.toString()); //output: 2023-02-27
```

```cpp
string str = sfput("The '{}' lib is written in {}.", "cpplib-core", "C++");
println(str); //output: The 'cpplib-core' lib is written in C++.
```

## Contributing
 Contributions are welcome to the cpplib-core library! If you have a bugfix or new feature, please create a pull request. If you have any questions, feel free to open an issue.