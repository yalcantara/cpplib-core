#pragma once

#include <sstream>
#include <optional>
#include <algorithm>
#include <ylib/core/lang.h>

namespace ylib::core {

using namespace std;

string fieldToString(const char *field, const optional<string> &val) {
    if (val.has_value() == true) {
        stringstream ss;
        ss << field;
        ss << ": '";
        ss << val.value();
        ss << "'";
        return ss.str();
    }
    
    stringstream ss;
    ss << field;
    ss << ": ";
    ss << "(unassigned)";
    return ss.str();
}

string fieldToString(const char *field, const optional<double> &val) {
    if (val.has_value() == true) {
        stringstream ss;
        ss << field;
        ss << ": ";
        ss << val.value();
        return ss.str();
    }
    
    stringstream ss;
    ss << field;
    ss << ": ";
    ss << "(unassigned)";
    return ss.str();
}

void findAndReplaceInplace(string &txt, const string &search, const string &replace) {
    size_t pos = 0;
    while ((pos = txt.find(search, pos)) != string::npos) {
        txt.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

string findAndReplace(const string &txt, const string &search, const string &replace) {
    string copy = txt;
    findAndReplaceInplace(copy, search, replace);
    return copy;
}

Bool endsWith(string &txt, const char c) {
    if (txt.length() == 0) {
        return False;
    }
    
    size_t idx = txt.length() - 1;
    if (txt.at(idx) == c) {
        return True;
    }
    
    return False;
}


Bool endsWith(const std::string &str, const std::string &suffix) {
    if (str.size() >= suffix.size() &&
        0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix)) {
        return True;
    }
    
    return False;
}

Bool endsWith(const std::string &str, const char *suffix) {
    std::string s = suffix;
    return endsWith(str, s);
}


string repeat(Int16 count, const char c) {
    stringstream ss;
    
    for (Int16 i = 0; i < count; i++) {
        ss << c;
    }
    
    return ss.str();
}

string numFill(Int64 num, UInt16 count, const char c) {
    checkParamIsPositive("count", count);
    
    string snum = std::to_string(num);
    
    size_t len = snum.length();
    
    if (len >= (size_t) count) {
        //nothing to fill.
        return snum;
    }
    
    size_t diff = count - len;
    stringstream ss;
    
    
    for (size_t i = 0; i < diff; i++) {
        ss << c;
    }
    
    ss << snum;
    return ss.str();
}

void toLowerInplace(string &txt) {
    
    std::transform(txt.begin(), txt.end(), txt.begin(), [](int c) {
        return std::tolower(c);
    });
}

string toLower(const string &txt) {
    string ans = txt;
    
    std::transform(ans.begin(), ans.end(), ans.begin(), [](int c) {
        return std::tolower(c);
    });
    
    return ans;
}

string toLower(const char *txt) {
    string stxt{txt};
    return toLower(stxt);
}

string format(double val, UInt16 precision) {
    
    std::stringstream ss;
    ss.precision(precision);
    ss << std::fixed << val;
    return ss.str();
}

class ToStringBuilder {
private:
    vector<string> keys;
    vector<string> values;
    string unassignedStr;
    UInt16 precision;
    
    void doAppend(string key, string val) {
        keys.push_back(key);
        values.push_back(val);
    }

public:
    
    ToStringBuilder(string unassignedStr, UInt16 precision) : unassignedStr(unassignedStr), precision(precision) {
    
    }
    
    ToStringBuilder() : unassignedStr("(unassigned)"), precision(2) {
    
    }
    
    void append(const char *key, const string &val, Bool addQuote) {
        if (addQuote == True) {
            string sval = "\"" + val + "\"";
            doAppend(string(key), sval);
        } else {
            doAppend(string(key), val);
        }
    }
    
    void append(const char *key, const string &val) {
        append(key, val, True);
    }
    
    void append(const string &key, const string &val, Bool addQuote) {
        if (addQuote == True) {
            string sval = "\"" + val + "\"";
            doAppend(key, sval);
        } else {
            doAppend(string(key), val);
        }
    }
    
    void append(const string &key, const string &val) {
        append(key, val, True);
    }
    
    void append(const char *key, UInt64 val) {
        doAppend(string(key), std::to_string(val));
    }
    
    void append(const char *key, Int64 val) {
        doAppend(string(key), std::to_string(val));
    }
    
    void append(const string &key, Int64 val) {
        doAppend(string(key), std::to_string(val));
    }
    
    void append(const string &key, const vector<string> &val) {
        doAppend(string(key), vecToString(val, True));
    }
    
    void appendOpt(const char *key, const optional<string> &val) {
        if (val) {
            append(key, val.value());
        } else {
            append(key, unassignedStr, False);
        }
    }
    
    void appendOpt(const char *key, const optional<Int64> &val) {
        if (val) {
            append(key, val.value());
        } else {
            append(key, unassignedStr, False);
        }
    }
    
    void appendOpt(const char *key, const optional<double> &val) {
        if (val) {
            append(key, format(val.value(), precision), False);
        } else {
            append(key, unassignedStr, False);
        }
    }
    
    void appendOpt(const char *key, const optional<vector<string>> &val) {
        if (val) {
            append(key, val.value());
        } else {
            append(key, unassignedStr, False);
        }
    }
    
    template<typename T, typename I>
    void appendOpt(const char *key, optional<I> val, T supplier) {
        if (val) {
            append(key, supplier(val.value()));
        } else {
            append(key, unassignedStr, False);
        }
    }
    
    string str() {
        size_t len = keys.size();
        ostringstream oss;
        oss << "{";
        for (size_t i = 0; i < len; i++) {
            string &k = keys[i];
            string &v = values[i];
            
            oss << k;
            oss << ": ";
            oss << v;
            
            if (i + 1 < len) {
                oss << ", ";
            }
        }
        oss << "}";
        return oss.str();
    }
};
}
