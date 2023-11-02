#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <cstdint>
#include <utility>
#include <optional>
#include <any>

using namespace std;


#define MAX_INT8_VAL 127
#define MIN_INT8_VAL -128

#ifndef Int8
#define Int8 int8_t
#endif // !Int8


#ifndef UInt8
#define UInt8 uint8_t
#endif // !UInt8

#ifndef Int16
#define Int16 int16_t
#endif // !Int16


#ifndef UInt16
#define UInt16 uint16_t
#endif // !UInt16


#ifndef Int32
#define Int32 int32_t
#endif // !Int32

#ifndef UInt32
#define UInt32 uint32_t
#endif // !UInt32


#ifndef Int64
#define Int64 int64_t
#endif // !Int64

#ifndef UInt64
#define UInt64 uint64_t
#endif // !Int64


/*
A convenient class that maps to C++ bool in a type-safe way. Provides the 
constant True and False, for clarity and consistency.
*/
class Bool {
private:
    bool _val;

    Bool(bool val) : _val{val} {
    }

    Bool() : Bool(false) {

    }

public:
    static Bool falseVal() {
        return Bool(false);
    }

    static Bool trueVal() {
        return Bool(true);
    }

    bool val() const {
        return _val;
    }

    string toString() const {
        if (_val) {
            return "True";
        }
        return "False";
    }
};

static const Bool True = Bool::trueVal();
static const Bool False = Bool::falseVal();

bool operator==(Bool &a, Bool &b) {
    return a.val() == b.val();
}

bool operator==(Bool a, const Bool b) {
    return a.val() == b.val();
}

bool operator!=(Bool &a, Bool &b) {
    return a.val() != b.val();
}

bool operator!=(Bool a, const Bool b) {
    return a.val() != b.val();
}


// Core println
//====================================================
void print(char c) {
    fputc(c, stdout);
    fflush(stdout);
}

void print(const char *str) {
    fputs(str, stdout);
    fflush(stdout);
}

void print(float val) {
    fprintf(stdout, "%.2f", val);
    fflush(stdout);
}

void print(string &str) {
    const char *ptr = str.c_str();
    fputs(ptr, stdout);
    fflush(stdout);
}

void println() {
    fputc('\n', stdout);
    fflush(stdout);
}

void println(char c) {
    fputc(c, stdout);
    fputc('\n', stdout);
    fflush(stdout);
}

void println(int val) {
    fprintf(stdout, "%d\n", val);
    fflush(stdout);
}

void println(long long val) {
    fprintf(stdout, "%lld\n", val);
    fflush(stdout);
}

void println(unsigned long long val) {
    fprintf(stdout, "%llu\n", val);
    fflush(stdout);
}

void println(unsigned int val) {
    fprintf(stdout, "%u\n", val);
    fflush(stdout);
}

void println(float val) {
    printf("%.2f\n", val);
    fflush(stdout);
}

void println(double val) {
    printf("%.2f\n", val);
    fflush(stdout);
}

void println(const char *str, ...) {
    va_list args;
    va_start (args, str);
    vfprintf(stdout, str, args);
    va_end(args);
    fputc('\n', stdout);
    fflush(stdout);
}

void println(std::string str) {
    const char *ptr = str.c_str();
    fputs(ptr, stdout);
    fputc('\n', stdout);
    fflush(stdout);
}
//====================================================


//Exception
//====================================================
class Exception : public exception {
protected:
    string _msg;

public:
    Exception() : _msg("") {

    }

    Exception(string msg) : _msg(msg) {

    }

    Exception(const char *msg) :
            _msg(msg) {

    }

    virtual const char *what() const throw() {
        return _msg.c_str();
    }

    virtual ~Exception() {

    }
};
//====================================================


//Generic Utils
//====================================================

Bool isNull(const void *ptr) {
    if (ptr == nullptr || ptr == NULL) {
        return True;
    }

    return False;
}

template<typename T>
bool is_primitive() {
    return is_arithmetic<T>::value || is_same<T, bool>::value;
}

template<typename T>
string anyToStr(T &ref) {

    T *ptr = &ref;
    if (ptr == nullptr) {
        return "nullptr";
    }

    if (is_same<T, bool>::value || is_same<T, const bool>::value) {
        bool casted = *((bool *) ptr);
        if (casted) {
            return "true";
        }
        return "false";
    }

    if (is_same<T, Bool>::value || is_same<T, const Bool>::value) {
        Bool casted = *((Bool *) ptr);
        if (casted == True) {
            return "True";
        }
        return "False";
    }

    if (is_integral<T>::value) {

        if (is_unsigned<T>::value) {
            if (is_same<T, unsigned int>::value) {
                return std::to_string(*((unsigned int *) ptr));
            }
            return std::to_string(*((unsigned long *) ptr));
        }

        return std::to_string(*((long long *) ptr));
    }

    if (is_floating_point<T>::value) {
        return std::to_string(*((double *) ptr));
    }

    if (is_same<T, size_t>::value || is_same<T, const size_t>::value) {
        return std::to_string(*((size_t *) ptr));
    }

    if (is_same<T, string>::value || is_same<T, const string>::value) {
        return *((string *) ptr);
    }

    if (is_same<T, const char *>::value) {
        std::any val = ref;
        return {any_cast<const char *>(val)};
    }

    if (is_array<T>::value) {
        return "[array]";
    }

    if (is_pointer<T>::value) {
        return "[pointer]";
    }

    if (is_class<T>::value) {
        return "[class]";
    }

    return "[other]";
}

template<typename T>
string sfput(string &txt, T arg) {

    size_t len = txt.length();

    if (len == 0) {
        return "";
    }

    const char *KEY = "{}";
    size_t KEYLEN = 2; // strlen(KEY)

    size_t idx = txt.find(KEY);

    if (idx == string::npos) {
        string copy{txt};
        return copy;
    }

    std::stringstream ss;

    size_t i = 0;
    for (; i < idx; i++) {
        ss << txt.at(i);
    }


    // Conversion
    // ============================================
    if (is_same<T, unsigned char>::value) {
        //bug in macOs Clang
        unsigned char *ptr = (unsigned char *) &arg;
        ss << std::to_string(*ptr);
    } else {
        ss << anyToStr(arg);
    }
    // ============================================

    i = i + KEYLEN;
    for (; i < len; i++) {
        ss << txt.at(i);
    }

    return ss.str();
}

string sfput(string &txt) {
    string msg{txt};
    return msg;
}

template<typename T, typename... Args>
string sfput(string &txt, T rep, Args... arg) {
    string mid = sfput(txt, rep);
    return sfput(mid, arg...);
}

template<typename T, typename... Args>
string sfput(const char *txt, T rep, Args... arg) {
    string stxt{txt};
    string mid = sfput(stxt, rep);
    return sfput(mid, arg...);
}


//Method parameters validation
//=====================================================================
optional<pair<size_t, size_t>> __ylib_core_find_nonspace(const string &txt) {
    if (txt.length() == 0) {
        return pair{0, 0};
    }

    size_t len = txt.length();
    size_t start = len;
    for (size_t i = 0; i < len; i++) {
        char c = txt.at(i);

        if (std::isspace(c) == false) {
            start = i;
            break;
        }
    }

    if(start == len){
        return nullopt;
    }

    size_t end = 0;
    for (size_t i = len - 1; i > start; i--) {

        char c = txt.at(i);

        if (std::isspace(c) == false) {
            end = i + 1;
            break;
        }
    }

    if(end == 0){
        //it means, it got never assigned in the loop, that is no whitespaces from the left
        return pair{start, len};
    }

    return pair{start, end};
}

string __ylib_core_trim(const string &txt) {

    auto nonSpaceOpt = __ylib_core_find_nonspace(txt);

    if (nonSpaceOpt.has_value() == false) {
        return std::string();
    }

    auto [start, end] = nonSpaceOpt.value();

    if (start > end) {
        //this case should not happen, but let's protect ourself.
        return {};
    }

    size_t diff = end - start;
    return txt.substr(start, diff);
}

string trim(const string &txt) {
    return __ylib_core_trim(txt);
}

string trim(const string &&txt) {
    return __ylib_core_trim(txt);
}

Bool isStrBlank(const char *s) {

    if (isNull(s) == True) {
        return True;
    }

    for (size_t i = 0;; i++) {
        char c = s[i];
        if (c == '\0') {
            return True;
        }

        if (std::isblank(c) == false) {
            return False;
        }
    }
    // unreachable code
}

Bool isStrBlank(string &s) {
    return isStrBlank(s.c_str());
}

template<typename T>
void checkParamNotNull(const char *name, T *val) {
    if (val == nullptr || val == NULL) {
        throw Exception(sfput("The param {} can not be empty or null. Got nullptr.", name));
    }
}

void checkParamNotEmpty(const char *name, string &val) {
    string *ptr = &val;
    if (ptr == nullptr || ptr == NULL) {
        throw Exception(sfput("The param {} can not be empty or null. Got nullptr.", name));
    }

    if (val.length() == 0) {
        throw Exception(sfput("The param {} can not be empty. Got ''.", name));
    }

    if (isStrBlank(val) == True) {
        throw Exception(sfput("The param {} can not be empty. Got string with whitespaces.", name));
    }
}

void checkParamNotEmpty(const char *name, const char *val) {
    if (val == nullptr || val == NULL) {
        throw Exception(sfput("The param {} can not be empty or null. Got nullptr.", name));
    }
    string sval{val};
    checkParamNotEmpty(name, sval);
}

template<typename T>
void checkParamBetween(const char *name, T val, T min, T max) {
    if (val < min || val > max) {
        throw Exception(
                sfput("The param {} must be between (inclusive) {} and {}. Got: {}.",
                      name, min, max, val));
    }
}

void checkParamBetween(const char *name, Int64 val, Int64 min, Int64 max) {
    if (val < min || val > max) {
        throw Exception(
                sfput("The param {} must be between (inclusive) {} and {}. Got: {}.",
                      name, min, max, val));
    }
}

void checkParamEqualOrHigherThan(const char *name, Int64 val, Int64 min) {
    if (val < min) {
        throw Exception(
                sfput("The param {} must be equal or higher than {}. Got: {}.",
                      name, min, val));
    }
}

//Positive is > 0.
void checkParamIsPositive(const char *name, Int64 val) {
    if (val <= 0) {
        throw Exception(
                sfput("The param {} must be positive (higher than zero). Got: {}.",
                      name, val));
    }
}
//====================================================


// Class printing
//====================================================
void println(const Exception &ex) {
    const char *msg = ex.what();
    string smsg{msg};
    string name = typeid(ex).name();
    string txt = "Exception thrown " + name + ": " + smsg;
    println(txt);
}

void print(ostringstream &oss, const vector<string> &vec, Bool horizontal) {


    std::size_t size = vec.size();

    oss << "[";
    for (size_t i = 0; i < size; i++) {
        const string &e = vec[i];
        oss << e;
        if ((i + 1) < size) {
            if (horizontal == True) {
                oss << ", ";
            } else {
                oss << "," << std::endl;
            }
        }
    }
    oss << "]";
}

string vec_to_string(const vector<string> &vec, Bool horizontal) {
    ostringstream oss;
    print(oss, vec, horizontal);
    return oss.str();
}

void println(const vector<string> &vec, Bool horizontal) {
    println(vec_to_string(vec, horizontal));
}

void println(const vector<string> &vec) {
    println(vec, True);
}
//====================================================

// Program Arguments
//====================================================
vector<string> argToVec(UInt8 argc, char **argv, UInt8 minRequired) {
    if (argc < minRequired) {
        throw Exception(sfput("The minimun required arguments is {}, but got {} instead.", minRequired, argc));
    }

    vector<string> ans;
    for (UInt8 i = 0; i < argc; i++) {
        const char *arg = argv[i];
        if (isStrBlank(arg) == True) {
            throw Exception(sfput("The argument at index {} is empty or blank.", i));
        }
        string sarg{arg};
        ans.push_back(sarg);
    }

    return ans;
}
//====================================================

// Environment
//====================================================
optional<string> getEnvOpt(const char *name) {
    const char *str = std::getenv(name);
    if (isNull(str) == True) {
        return std::nullopt;
    }
    return string{str};
}

string checkAndGetEnv(const char *name) {
    auto envOpt = getEnvOpt(name);
    if (envOpt.has_value()) {
        return envOpt.value();
    }
    throw Exception(sfput("The environment variable {} does not exist or might be empty.", name));
}
//====================================================