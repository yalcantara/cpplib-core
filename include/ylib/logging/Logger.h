#pragma once

#include <ylib/core/lang.h>
#include <ylib/core/time.h>
#include <ylib/io/io.h>
#include <ylib/thread/Sync.h>


namespace ylib::logging {

using namespace ylib::core;
using ylib::tread::Sync;

const char* CPPLIB_LOGGER_DEFAULT_OUT_DIR = "logs/";
Int8 CPPLIB_LOGGER_MAX_NAME_LENGTH = 0;

//Mutexes
//Not a Read-Write lock mechanics. Just 2 simple lock:
//1. When performing the actual write to output.
//2. When retrieving an instance of Logger.
static std::mutex CPPLIB_LOGGER_WRITE_LOCK;
static std::mutex CPPLIB_LOGGER_GET_LOCK;

class Entry {
public:
    string level;
    LocalDateTime time; //local time
    string msg;
};


/*
A very simplistic logger implementation that is also, thread safe. All writes
immediately flushes to the disk avoiding loss of messages in memory. Outputs the
log files to the logs/ directory, using current day a rolling appender.
*/
class Logger {
private:
	const string _name;
    const string _outDir;
    
    Logger(string name, string outDir) : _name{ name }, _outDir{outDir} {
    }

    //The parameter entry has to be by-value to be sure we have a copy while writing 
    //the contents. This prevents any modification in a race condition.
    void write(Entry entry) {

        //Unique method that actually writes the content of entry.
        //It uses locking mechanism to prevent data race condition
        //that may affect log order.

        //Synchronization
        Sync sync{CPPLIB_LOGGER_WRITE_LOCK};

        string level = entry.level;
        string time = entry.time.time().toString();
        string msg = entry.msg;
        
        stringstream ss;
        ss << level;
        ss << "  ";
        ss << time;
        ss << "  [";

        // We already checked that the name length is <= INT8_MAX int the method:
        // Logger::get
        Int8 len = (Int8)_name.length();
        if (len >= CPPLIB_LOGGER_MAX_NAME_LENGTH) {
            ss << _name;
        } else {
            ss << _name;
            // A compiler warning: the - operator returns an int. Here by this 
            // casting we are just making sure we are back to Int8.
            Int8 diff = (Int8)(CPPLIB_LOGGER_MAX_NAME_LENGTH - len);
            ss << repeat(diff, ' ');
        }
        ss << "] - ";
        ss << msg;

        string txt = ss.str();

        string date = entry.time.date().toString();
        string file = date + ".log";

        string soutdir = _outDir;
        ylib::io::writeln(soutdir, file, "ab", txt);
        println(txt);
    }

public:


    static const Logger get(const char* name) {
        checkParamNotEmpty("name", name);

        string sname{ name };
        size_t len = sname.length();
        checkParamBetween("name.length", len, 1, INT8_MAX);

        // Synchronization to prevent race condition
        Sync sync{CPPLIB_LOGGER_GET_LOCK };
        CPPLIB_LOGGER_MAX_NAME_LENGTH = std::max(CPPLIB_LOGGER_MAX_NAME_LENGTH, (Int8)len);

        return { sname , CPPLIB_LOGGER_DEFAULT_OUT_DIR};
    }

    // Rule of five
    // =========================================================================
    // 1. Copy Constructor
    // No copy constructor allowed
    Logger(const Logger&) = delete;

    // 2. Copy Assignment
    // no copy assignment allowed
    Logger& operator=(const Logger& other) = delete;

    // 3. Move Constructor
    // move constructor allowed

    // 4. Move Assignment
    // No move assignment allowed

    // 5. Destructor
    // No need to implement
    // =========================================================================


    void info(const string& msg) {

        Entry entry;
        entry.level = "INFO "; //extra space for padding with ERROR
        entry.msg = msg;

        write(entry);
    }

    void info(string&& msg) {
        string ref{ msg };
        info(ref);
    }

    void info(const char* msg) {
        string txt{ msg };
        info(txt);
    }

    void error(const string& msg) {
        Entry entry;
        entry.level = "ERROR";
        entry.msg = msg;

        write(entry);
    }

    void error(string&& msg) {
        string ref = msg;
        error(ref);
    }

    void error(const std::exception& ex) {
        const char* msg = ex.what();
        string smsg{ msg };
        string name = typeid(ex).name();
        string txt = "Exception thrown " + name +": " + smsg;
        error(txt);
    }

    void warn(const string& msg) {
        Entry entry;
        entry.level = "WARN "; //extra space for padding with ERROR
        entry.msg = msg;

        write(entry);
    }

    void warn(const char* msg){
        string smsg = msg;
        warn(smsg);
    }
};
}