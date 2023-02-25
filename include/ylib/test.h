//
// Created by Yaison Alcantara 3 on 2/20/23.
//

#pragma once

#include <iostream>
#include <functional>
#include <cstring>

namespace ylib::test {
    using namespace std;
    using namespace ylib::core;

    class AssertException : public Exception {
    public:
        AssertException(const char *msg) : Exception(msg) {

        }

        AssertException(string msg) : Exception(msg) {

        }
    };

    string cpplib_core_failure_msg(const char* name, exception& ex){
        ostringstream oss;
        oss << "\x1B[31mFAILURE:\033[0m ";
        oss << name;
        oss << " - Exception: ";
        oss << ex.what();

        return oss.str();
    }

    string cpplib_core_success_msg(const char* name){
        ostringstream oss;
        oss << "\x1B[32mSUCCESS:\033[0m ";
        oss << name;
        return oss.str();
    }

    void cpplib_core_assert_failed(string expected, string actual){
        ostringstream oss;
        oss << "Assert failed. Should be '";
        oss << expected;
        oss << "', but got '";
        oss << actual;
        oss << "' instead.";
        throw AssertException(oss.str());
    }

    void assertTrue(Bool ans) {
        if (ans != True) {
            throw AssertException("Assert failed. Not True.");
        }
    }

    void assertTrue(bool ans) {
        if (ans != true) {
            throw AssertException("Assert failed. Not true.");
        }
    }

    void assertFalse(bool ans) {
        if (ans != false) {
            throw AssertException("Assert failed. Not true.");
        }
    }


    void assertEquals(const char* expected, string actual) {

        if (strcmp(expected, actual.c_str()) != 0) {
            cpplib_core_assert_failed({expected}, actual);
        }
    }

    void assertEquals(Int64 expected, Int64 actual){
        if(expected != actual){
            cpplib_core_assert_failed(std::to_string(expected), std::to_string(actual));
        }
    }

    void assertEquals(Month expected, Month actual){
        if(expected != actual){
            cpplib_core_assert_failed(monthToString(expected), monthToString(actual));
        }
    }




    void TEST(const char *name, std::function<void()> f) {
        try {
            f();
            cout << cpplib_core_success_msg(name) << endl;
        }catch (AssertException& ex){
            cout << cpplib_core_failure_msg(name, ex) << endl;
        }catch (exception& ex){
            cout << cpplib_core_failure_msg(name, ex) << endl;
        }
    }
}
