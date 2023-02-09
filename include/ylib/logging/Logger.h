#pragma once

#include <ylib/core/lang.h>
#include <ylib/core/time.h>
#include <ylib/io/io.h>
#include <ylib/thread/Sync.h>


namespace ylib {
namespace logging {

using namespace ylib::core;
using ylib::tread::Sync;

const char* DEFAULT_OUT_DIR = "logs/";
Int8 __maxNameLen = 0;

//Mutexes
//Not a Read-Write lock mechanics. Just 2 simple lock:
//1. When performing the actual write to output.
//2. When retrieving an instance of Logger.
std::mutex __logger_write_lock;
std::mutex __logger_get_lock;


class Entry {
public:
    string level;
    LocalDateTime time; //local time
    string msg;
};



class Logger {
private:
	const string _name;
    const string _outDir;
    


    Logger(string name, string outDir) : _name{ name }, _outDir{outDir} {
    }



  
    //The parameter entry has to be by-value to be sure we have a copy while writing 
    //the contents. This prevent any modification in a race condition.
    void write(Entry entry) {

        //Unique method that actually writes the content of entry.
        //It uses locking mechanism to prevent data race condition
        //that may affect log order.

        //Synchronization
        Sync sync{ __logger_write_lock };

        string level = entry.level;
        string time = entry.time.time().toString();
        string msg = entry.msg;
        
        stringstream ss;
        ss << level;
        ss << "  ";
        ss << time;
        ss << "  [";

        //we already checked that the name length is <= INT8_MAX int the method:
        //Logger::get
        Int8 len = (Int8)_name.length();
        if (len >= __maxNameLen) {
            ss << _name;
        } else {
            ss << _name;
            //A compiler warning: the - operator returns an int. Here by this 
            //casting we are just making sure we are back to Int8.
            Int8 diff = (Int8)(__maxNameLen - len);
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


        //Synchronization to prevent race condition
        Sync sync{ __logger_get_lock };
        __maxNameLen = std::max(__maxNameLen, (Int8)len);

        Logger ans{ sname , DEFAULT_OUT_DIR};
        return ans;
    }

    //2. Copy Assignment
    //no copy assignment allowed
    Logger& operator=(const Logger& other) = delete;



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
}