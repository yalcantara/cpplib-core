#pragma once

#include <ylib/core/time.h>
#include <ylib/test.h>

namespace ylib::core {

using namespace ylib::core;
using namespace ylib::test;

void testTime() {
    TEST("test DateTime", [] {
        
        // Given
        Int64 unixMilliTime = 1'676'946'245'082;
        Int64 zeroDawn = 0;
        Int64 IXX = -2208988800 * 1000; // 1900-01-01 00:00:00.000
        Int64 year50 = 2550193082999; //2050-10-24 02:58:02.999
        
        // When
        auto t1 = DateTime::from(unixMilliTime);
        auto t2 = DateTime::from(zeroDawn);
        auto t3 = DateTime::from(IXX);
        auto t4 = DateTime::from(year50);
        
        // Then
        assertEquals(2023, t1.date().year());
        assertEquals(21, t1.date().day());
        assertEquals("2023-02-21 02:24:05.082", t1.toString());
        
        assertEquals(1970, t2.date().year());
        assertEquals(Month::January, t2.date().month());
        assertEquals("1970-01-01 00:00:00.000", t2.toString());
        
        assertEquals(1900, t3.date().year());
        assertEquals(Month::January, t3.date().month());
        assertEquals("1900-01-01 00:00:00.000", t3.toString());
        
        assertEquals(2050, t4.date().year());
        assertEquals(Month::October, t4.date().month());
        assertEquals("2050-10-24 02:58:02.999", t4.toString());
    });
}
} //ylib::core
