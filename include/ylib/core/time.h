#pragma once

#include <chrono>
#include <tuple>
#include <ylib/core/lang.h>
#include <ylib/core/sutils.h>

namespace ylib::core {

using std::tuple;
using std::chrono::duration_cast;
using std::chrono::system_clock;

// In C the minimum time is: 1900-01-01 00:00:00 which in seconds from 1970 is: -2208988800
const Int64 MINIMUN_EPOCH_C_TIME_SECONDS = -2208988800;

enum class Month {
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

bool operator==(UInt8 a, Month b) {
    return a == static_cast<int>(b);
}

bool operator==(Month b, UInt8 a) {
    return a == b;
}

bool operator!=(Month b, UInt8 a) {
    if (b == a) {
        return false;
    }
    
    return true;
}

string monthToString(Month month) {
    
    switch (month) {
        case Month::January:
            return "January";
        case Month::February:
            return "February";
        case Month::March:
            return "March";
        case Month::April:
            return "April";
        case Month::May:
            return "May";
        case Month::June:
            return "June";
        case Month::July:
            return "July";
        case Month::August:
            return "August";
        case Month::September:
            return "September";
        case Month::October:
            return "October";
        case Month::November:
            return "November";
        case Month::December:
            return "December";
        default:
            throw Exception("Unsupported month."); // shouldn't happen
    }
}

UInt8 monthToUInt(Month month) {
    return static_cast<int>(month) + 1;
}

Month uintToMonth(UInt8 month) {
    checkParamBetween("month", month, 1, 12);
    return static_cast<Month>(month - 1);
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
    Int16 milli = (Int16) (milliDuration % (Int16) 1000);
    
    time_t timeT = system_clock::to_time_t(now);
    
    if (gmt == True) {
        tm ctm = *gmtime(&timeT);
        return std::make_tuple(ctm, milli);
    }
    
    tm ctm = *localtime(&timeT);
    return std::make_tuple(ctm, milli);
}

Bool isLeap(Int32 year) {
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        return True;
    }
    return False;
}

Bool is30Month(Month month) {
    if (month == Month::April ||
        month == Month::June ||
        month == Month::September ||
        month == Month::November) {
        return True;
    }
    return False;
}

/*
Represents a date in the Gregorian calendar, without time. The Date class assumes GMT-0 locale.  
*/
class Date {
private:
    Int32 _year;
    Month _month;
    UInt8 _day;
    
    void validate() {
        checkParamBetween("day", _day, 1, 31); //just a quick check
        
        if (_month == Month::February) {
            if (ylib::core::isLeap(_year) == False && _day > 28) {
                throw Exception(sfput("Invalid day field for month february. \
                                        Got: {}, expected [1, 28].", _day));
            }
        }
        
        if (ylib::core::is30Month(_month) == True && _day > 30) {
            throw Exception(sfput("Invalid date for month {}. Expected \
                    between 1 and 30, but got: {} instead.", monthToString(_month), _day));
            
        }
    }
    
    void init(tm t) {
        _year = (Int32) (1900 + t.tm_year);
        _month = uintToMonth(t.tm_mon + 1);
        _day = (Int8) t.tm_mday;
        validate();
    }

public:
    
    static Date today() {
        return Date();
    }
    
    static Date yesterday() {
        time_t y = time(NULL) - (60 * 60 * 24);
        tm t = *gmtime(&y);
        return Date(t);
    }
    
    Date(tm t) {
        init(t);
    }
    
    Date() {
        init(ctimeGMT());
    }
    
    Date(Int32 year, UInt8 month, UInt8 day) {
        _year = year;
        _month = uintToMonth(month);
        _day = day;
        validate();
    }
    
    Int32 year() const {
        return _year;
    }
    
    Month month() const {
        return _month;
    }
    
    UInt8 day() const {
        return _day;
    }
    
    Bool isLeap() {
        return ylib::core::isLeap(_year);
    }
    
    time_t toUnixTime() {
        tm t = ctimeGMT();
        t.tm_year = _year - 1900;
        t.tm_mon = monthToUInt(_month) - 1;
        t.tm_mday = _day;
        t.tm_hour = 0;
        t.tm_min = 0;
        t.tm_sec = 0;
        
        time_t ans = timegm(&t);
        return ans;
    }
    
    Date addDays(Int32 days) {
        if (days == 0) {
            return *this;
        }
        
        time_t epoch = toUnixTime() + (60 * 60 * 24 * days);
        tm t = *gmtime(&epoch);
        return Date(t);
    }
    
    string toString() const {
        return sfput("{}-{}-{}", _year,
                     numFill(monthToUInt(_month), 2, '0'),
                     numFill(_day, 2, '0'));
    }
};

/*
Represents a date in the Gregorian calendar, without time using the current system locale.  
*/
class LocalDate {
private:
    Int32 _year;
    Month _month;
    UInt8 _day;
    
    void validate() {
        checkParamBetween("day", _day, 1, 31); //just a quick check
        
        if (_month == Month::February) {
            if (ylib::core::isLeap(_year) == False && _day > 28) {
                throw Exception(sfput("Invalid day field for month february. \
                                        Got: {}, expected [1, 28].", _day));
            }
        }
        
        if (ylib::core::is30Month(_month) == True && _day > 30) {
            throw Exception(sfput("Invalid date for month {}. Expected \
                    between 1 and 30, but got: {} instead.", monthToString(_month), _day));
            
        }
    }
    
    void init(const tm &t) {
        _year = (Int32) (1900 + t.tm_year);
        _month = uintToMonth(t.tm_mon + 1);
        _day = (Int8) t.tm_mday;
        validate();
    }

public:
    
    LocalDate(tm &t) {
        init(t);
    }
    
    LocalDate() {
        init(ctimeLocal());
    }
    
    LocalDate(Int32 year, UInt8 month, UInt8 day) {
        _year = year;
        _month = uintToMonth(month);
        _day = day;
        validate();
    }
    
    Int32 year() const {
        return _year;
    }
    
    Month month() const {
        return _month;
    }
    
    UInt8 day() const {
        return _day;
    }
    
    Bool isLeap() {
        return ylib::core::isLeap(_year);
    }
    
    string toString() const {
        stringstream ss;
        ss << _year;
        ss << '-';
        ss << numFill(monthToUInt(_month), 2, '0');
        ss << '-';
        ss << numFill(_day, 2, '0');
        return ss.str();
    }
};

/*
Represents the current time at GMT-0 up to milliseconds resolution.
*/
class Time {

private:
    UInt8 _hour;
    UInt8 _min;
    UInt8 _sec;
    UInt16 _milli;
    
    void validate() {
        checkParamBetween("hour", _hour, 0, 23);
        checkParamBetween("min", _min, 0, 59);
        checkParamBetween("sec", _sec, 0, 59);
        checkParamBetween("milli", _milli, 0, MILLI_MAX);
    }

public:
    
    static const UInt16 MILLI_MAX = 999;
    
    Time(UInt8 hour, UInt8 min, UInt8 sec, UInt16 milli) :
            _hour{hour},
            _min{min},
            _sec{sec},
            _milli{milli} {
        validate();
    }
    
    Time(tm &t, UInt16 milli) : Time((UInt8) t.tm_hour, (UInt8) t.tm_min, (UInt8) t.tm_sec, milli) {
    }
    
    Time(UInt8 hour, UInt8 min, UInt8 sec) : Time(hour, min, sec, 0) {
    }
    
    Time() {
        auto [ctm, milli] = instant(True);
        
        _hour = (Int8) ctm.tm_hour;
        _min = (Int8) ctm.tm_min;
        _sec = (Int8) ctm.tm_sec;
        _milli = milli;
        
        validate();
    }
    
    UInt8 hour() const {
        return _hour;
    }
    
    UInt8 min() const {
        return _min;
    }
    
    UInt8 sec() const {
        return _sec;
    }
    
    UInt16 milli() const {
        return _milli;
    }
    
    string toString() const {
        return sfput("{}:{}:{}.{}",
                     numFill(_hour, 2, '0'),
                     numFill(_min, 2, '0'),
                     numFill(_sec, 2, '0'),
                     numFill(_milli, 3, '0'));
    }
};


/*
Represents the current time, using the current system locale, up to milliseconds resolution.
*/
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
            _hour{hour},
            _min{min},
            _sec{sec},
            _milli{milli} {
        validate();
    }
    
    LocalTime(tm &t, Int16 milli) : LocalTime((Int8) t.tm_hour, (Int8) t.tm_min, (Int8) t.tm_sec, (Int16) milli) {
    }
    
    LocalTime(Int8 hour, Int8 min, Int8 sec) : LocalTime(hour, min, sec, 0) {
    }
    
    LocalTime() {
        auto [ctm, milli] = instant(False);
        
        _hour = (Int8) ctm.tm_hour;
        _min = (Int8) ctm.tm_min;
        _sec = (Int8) ctm.tm_sec;
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
        return sfput("{}:{}:{}.{}",
                     numFill(_hour, 2, '0'),
                     numFill(_min, 2, '0'),
                     numFill(_sec, 2, '0'),
                     numFill(_milli, 3, '0'));
    }
};


/*
Represents a date with time at GMT-0 locale, using the Date and Time classes.
*/
class DateTime {
private:
    Date _date;
    Time _time;

public:
    static DateTime now() {
        return {};
    }
    
    static DateTime from(Int64 millis) {
        checkParamEqualOrHigherThan("millis", millis, MINIMUN_EPOCH_C_TIME_SECONDS * 1000);
        time_t tt = (time_t) (millis / (time_t) 1000);
        
        UInt16 milliFraction = (UInt16) (std::abs(millis) % (UInt16) 1000);
        
        tm ctm = *gmtime(&tt);
        Date date{ctm};
        Time time{ctm, milliFraction};
        return DateTime(date, time);
    }
    
    DateTime(Date date, Time time) : _date{date}, _time{time} {
    
    }
    
    DateTime() {
        auto [ctm, milli] = instant(True);
        
        _date = Date(ctm);
        _time = Time(ctm, milli);
    }
    
    Date date() const {
        return _date;
    }
    
    Time time() const {
        return _time;
    }
    
    string toString() const {
        return sfput("{} {}", _date.toString(), _time.toString());
    }
};

/*
Represents a date with time using the current system locale, using the LocalDate and LocalTime classes.
*/
class LocalDateTime {
private:
    LocalDate _date;
    LocalTime _time;

public:
    static LocalDateTime now() {
        return {};
    }
    
    LocalDateTime(LocalDate date, LocalTime time) : _date{date}, _time{time} {
    
    }
    
    LocalDateTime() {
        auto [ctm, milli] = instant(False);
        
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
        return sfput("{} {}", _date.toString(), _time.toString());
    }
};


DateTime nowGMT() {
    return {};
}

LocalDateTime nowLocal() {
    return {};
}
}
