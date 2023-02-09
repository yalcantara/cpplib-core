#pragma once

#include <chrono>
#include <tuple>
#include <ylib/core/lang.h>
#include <ylib/core/sutils.h>

using std::tuple;
using std::chrono::duration_cast;
using std::chrono::system_clock;

namespace ylib {
namespace core {
	
enum Month {
    January,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

bool operator==(Int8 a, Month b) {
    if (a == 1 and b == Month::January) {
        return true;
    }

    if (a == 2 and b == Month::February) {
        return true;
    }

    if (a == 3 and b == Month::March) {
        return true;
    }

    if (a == 4 and b == Month::April) {
        return true;
    }

    if (a == 5 and b == Month::May) {
        return true;
    }

    if (a == 6 and b == Month::June) {
        return true;
    }

    if (a == 7 and b == Month::July) {
        return true;
    }

    if (a == 8 and b == Month::August) {
        return true;
    }

    if (a == 9 and b == Month::September) {
        return true;
    }

    if (a == 10 and b == Month::October) {
        return true;
    }

    if (a == 11 and b == Month::November) {
        return true;
    }

    if (a == 10 and b == Month::December) {
        return true;
    }

    return false;
}



tm ctimeGMT() {
    auto now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
    tm gmt = *gmtime(&tt);

    return gmt;
}

tm ctimeLocal() {
    auto now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
    tm gmt = *localtime(&tt);

    return gmt;
}

system_clock::duration timeSinceEpoch() {
    auto now = system_clock::now();
    return now.time_since_epoch();
}

Int64 timeSinceEpochMillis() {
    auto duration = timeSinceEpoch();
    Int64 millis = duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

tuple<tm, Int16> instant(Bool gmt) {
    auto now = system_clock::now();

    auto duration = now.time_since_epoch();
    Int64 milliDuration = duration_cast<std::chrono::milliseconds>(duration).count();
    Int16 milli = (Int16)(milliDuration % (Int16)1000);

    time_t timeT = system_clock::to_time_t(now);
    
    if (gmt == True) {
        tm ctm = *gmtime(&timeT);
        return std::make_tuple(ctm, milli);
    }

    tm ctm = *localtime(&timeT);
    return std::make_tuple(ctm, milli);
}

Bool isLeap(Int32 year) {
    if(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)){
        return True;
    }
    return False;
}

Bool is30Month(Int8 month) {
    if(     month == Month::April ||
            month == Month::June ||
            month == Month::September ||
            month == Month::November){
        return True;
    }
    return False;
}

class Date {
private:
    Int32 _year;
    Int8 _month;
    Int8 _day;

    void validate() {
        checkParamBetween("month", _month, 1, 12);
        checkParamBetween("day", _day, 1, 31); //just a quick check

        if (_month == Month::February) {
            if (ylib::core::isLeap(_year) == False && _day > 28) {
                throw Exception(sfput("Invalid day field for month february. \
                                        Got: ${}, expected [1, 28].", _day));
            }
        }

        if (ylib::core::is30Month(_month) == True && _day > 30) {
            throw Exception(sfput("Invalid date for month ${}. Expected \
                    between 1 and 30, but got: ${} instead.", _month, _day));
            
        }
    }

    void init(const tm& t) {
        _year = (Int32)(1900 + t.tm_year);
        _month = (Int8)(t.tm_mon + 1);
        _day = (Int8)t.tm_mday;
        validate();
    }

public:
    
    static Date today(){
        return Date();
    }

    static Date yesterday(){
        time_t y = time(NULL) - (60 * 60 * 24);
        tm t = *gmtime(&y);
        return Date(t);
    }


    Date(tm t) {
        init(t);
    }

    Date()  {
        init(ctimeGMT());
    }

    Date(Int32 year, Int8 month, Int8 day) {
        _year = year;
        _month = month;
        _day = day;
        validate();
    }

    Int32 year() const{
        return _year;
    }

    Int8 month() const {
        return _month;
    }

    Int8 day() const {
        return _day;
    }

    Bool isLeap() {
        return ylib::core::isLeap(_year);
    }

    time_t toUnixTime(){
        tm t = ctimeGMT();
        t.tm_year = _year - 1900;
        t.tm_mon = _month - 1;
        t.tm_mday = _day;
        t.tm_hour = 0;
        t.tm_min = 0;
        t.tm_sec = 0;

        time_t ans = timegm(&t);
        return ans;
    }

    Date addDays(Int32 days){
        if(days == 0){
            return *this;
        }

        time_t epoch = toUnixTime() + (60 * 60 * 24 * days);
        tm t = *gmtime(&epoch);
        return Date(t);
    }

    string toString() const {
        stringstream ss;
        ss << _year;
        ss << '-';
        ss << numFill(_month, 2, '0');
        ss << '-';
        ss << numFill(_day, 2, '0');
        return ss.str();
    }
};



class LocalDate {
private:
    Int32 _year;
    Int8 _month;
    Int8 _day;

    void validate() {
        checkParamBetween("month", _month, 1, 12);
        checkParamBetween("day", _day, 1, 31); //just a quick check

        if (_month == Month::February) {
            if (ylib::core::isLeap(_year) == False && _day > 28) {
                throw Exception(sfput("Invalid day field for month february. \
                                        Got: ${}, expected [1, 28].", _day));
            }
        }

        if (ylib::core::is30Month(_month) == True && _day > 30) {
            throw Exception(sfput("Invalid date for month ${}. Expected \
                    between 1 and 30, but got: ${} instead.", _month, _day));
            
        }
    }

    void init(const tm& t) {
        _year = (Int32)(1900 + t.tm_year);
        _month = (Int8)(t.tm_mon + 1);
        _day = (Int8)t.tm_mday;
        validate();
    }

public:
    
    LocalDate(tm& t) {
        init(t);
    }

    LocalDate()  {
        init(ctimeLocal());
    }

    LocalDate(Int32 year, Int8 month, Int8 day) {
        _year = year;
        _month = month;
        _day = day;
        validate();
    }

    Int32 year() const{
        return _year;
    }

    Int8 month() const {
        return _month;
    }

    Int8 day() const {
        return _day;
    }

    Bool isLeap() {
        return ylib::core::isLeap(_year);
    }

    string toString() const {
        stringstream ss;
        ss << _year;
        ss << '-';
        ss << numFill(_month, 2, '0');
        ss << '-';
        ss << numFill(_day, 2, '0');
        return ss.str();
    }
};






class Time {

private:
    Int8 _hour;
    Int8 _min;
    Int8 _sec;
    Int16 _milli;

    void validate() {
        checkParamBetween("hour", _hour, 0, 23);
        checkParamBetween("min", _min, 0, 59);
        checkParamBetween("sec", _sec, 0, 59);
        checkParamBetween("milli", _milli, 0, MILLI_MAX);
    }

    

public:

    static const Int16 MILLI_MAX = 999;

    

    Time(Int8 hour, Int8 min, Int8 sec, Int16 milli) :
        _hour{ hour },
        _min{ min },
        _sec{ sec },
        _milli{ milli } {
        validate();
    }

    Time(tm& t, Int16 milli) : Time((Int8)t.tm_hour, (Int8)t.tm_min, (Int8)t.tm_sec, (Int16)milli) {
    }

    Time(Int8 hour, Int8 min, Int8 sec) : Time(hour, min, sec, 0) {
    }

    

    Time() {
        tuple<tm, Int16> ins = instant(True);
      
        tm ctm = std::get<0>(ins);
        Int16 milli = std::get<1>(ins);
        
        _hour = (Int8)ctm.tm_hour;
        _min = (Int8)ctm.tm_min;
        _sec = (Int8)ctm.tm_sec;
        _milli = milli;

        validate();
    }

    
    Int8 hour() const {
        return _hour;
    }

    Int8 min() const {
        return _min;
    }

    Int8 sec() const {
        return _sec;
    }

    Int16 milli() const {
        return _milli;
    }


    string toString() const {
        stringstream ss;
        ss << numFill(_hour, 2, '0');
        ss << ':';
        ss << numFill(_min, 2, '0');
        ss << ':';
        ss << numFill(_sec, 2, '0');
        ss << '.';
        ss << numFill(_milli, 3, '0');
        return ss.str();
    }
};




class LocalTime {

private:
    Int8 _hour;
    Int8 _min;
    Int8 _sec;
    Int16 _milli;

    void validate() {
        checkParamBetween("hour", _hour, 0, 23);
        checkParamBetween("min", _min, 0, 59);
        checkParamBetween("sec", _sec, 0, 59);
        checkParamBetween("milli", _milli, 0, MILLI_MAX);
    }

    

public:

    static const Int16 MILLI_MAX = 999;

    

    LocalTime(Int8 hour, Int8 min, Int8 sec, Int16 milli) :
        _hour{ hour },
        _min{ min },
        _sec{ sec },
        _milli{ milli } {
        validate();
    }

    LocalTime(tm& t, Int16 milli) : LocalTime((Int8)t.tm_hour, (Int8)t.tm_min, (Int8)t.tm_sec, (Int16)milli) {
    }

    LocalTime(Int8 hour, Int8 min, Int8 sec) : LocalTime(hour, min, sec, 0) {
    }

    

    LocalTime() {
        tuple<tm, Int16> ins = instant(False);
      
        tm ctm = std::get<0>(ins);
        Int16 milli = std::get<1>(ins);
        
        _hour = (Int8)ctm.tm_hour;
        _min = (Int8)ctm.tm_min;
        _sec = (Int8)ctm.tm_sec;
        _milli = milli;

        validate();
    }

    
    Int8 hour() const {
        return _hour;
    }

    Int8 min() const {
        return _min;
    }

    Int8 sec() const {
        return _sec;
    }

    Int16 milli() const {
        return _milli;
    }


    string toString() const {
        stringstream ss;
        ss << numFill(_hour, 2, '0');
        ss << ':';
        ss << numFill(_min, 2, '0');
        ss << ':';
        ss << numFill(_sec, 2, '0');
        ss << '.';
        ss << numFill(_milli, 3, '0');
        return ss.str();
    }
};








class DateTime {
private:
    Date _date;
    Time _time;


public:
    static DateTime now(){
        return DateTime();
    }

    static DateTime from(Int64 millis){


        time_t tt = (millis / (time_t)1000);
        Int16 milli = (Int16)(millis % (Int16)1000);

    
        tm ctm = *gmtime(&tt);
        Date date{ctm};
        Time time{ctm, milli};
        return DateTime(date, time);
    }

    DateTime(Date date, Time time) :_date{ date }, _time{time} {
        
    }

    DateTime() {
        tuple<tm, Int16> ins = instant(True);
        tm ctm = std::get<0>(ins);
        Int16 milli = std::get<1>(ins);

        _date = Date(ctm);
        _time = Time(ctm, milli);
    }

    Date date() const {
        return _date;
    }

    Time time() const {
        return _time;
    }

    string toString() {
        stringstream ss;
        ss << _date.toString();
        ss << " ";
        ss << _time.toString();
        return ss.str();
    }
};

class LocalDateTime {
private:
    LocalDate _date;
    LocalTime _time;


public:
    static LocalDateTime now(){
        return LocalDateTime();
    }

    LocalDateTime(LocalDate date, LocalTime time) :_date{ date }, _time{time} {
        
    }

    LocalDateTime() {
        tuple<tm, Int16> ins = instant(False);
        tm ctm = std::get<0>(ins);
        Int16 milli = std::get<1>(ins);

        _date = LocalDate(ctm);
        _time = LocalTime(ctm, milli);
    }

    LocalDate date() const {
        return _date;
    }

    LocalTime time() const {
        return _time;
    }

    string toString() {
        stringstream ss;
        ss << _date.toString();
        ss << " ";
        ss << _time.toString();
        return ss.str();
    }
};


DateTime nowGMT() {

    DateTime ans;
    return ans;
}

LocalDateTime nowLocal() {

    LocalDateTime ans;
    return ans;
}



}
}